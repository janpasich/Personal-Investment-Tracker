#include "SaveManager.h"
#include <fstream>
#include <sstream>
#include <memory>
#include "Stock.h"
#include "PreciousMetal.h"
#include "Crypto.h"

void SaveManager::writeStringBin(std::ofstream& file, const std::string& str) {
    size_t length = str.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(str.c_str(), length);
}

std::string SaveManager::readStringBin(std::ifstream& file) {
    size_t length = 0;
    if (file.read(reinterpret_cast<char*>(&length), sizeof(length))) {
        std::string str(length, '\0');
        file.read(&str[0], length);
        return str;
    }
    return "";
}

bool SaveManager::savePortfolioBinary(const Portfolio& portfolio, Cash* cash, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    writeStringBin(file, cash->formatToCSV());

    const auto& assets = portfolio.getAssets();
    size_t count = assets.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& asset : assets) {
        writeStringBin(file, asset->formatToCSV());
    }

    file.close();
    return true;
}

bool SaveManager::loadPortfolioBinary(Portfolio& portfolio, Cash* cash, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    auto parseAsset = [&](const std::string& line) {
        std::stringstream ss(line);
        std::string type, name, quantityStr, averagePriceStr, currentPriceStr, dividendStr;
        std::getline(ss, type, ';');
        std::getline(ss, name, ';');
        std::getline(ss, quantityStr, ';');
        std::getline(ss, averagePriceStr, ';');
        std::getline(ss, currentPriceStr, ';');
        std::getline(ss, dividendStr, ';');

        double quantity = std::stod(quantityStr);
        double averagePrice = std::stod(averagePriceStr);
        double currentPrice = std::stod(currentPriceStr);
        double dividend = dividendStr.empty() ? 0.0 : std::stod(dividendStr);

        if (type == "Cash") {
            double currentBalance = cash->getQuantity();
            cash->subtractFromPosition(currentBalance);
            cash->addFunds(quantity);
        }
        else if (type == "Stock") {
            portfolio.addAsset(std::make_unique<Stock>(name, quantity, averagePrice, currentPrice, dividend));
        }
        else if (type == "PreciousMetal") {
            portfolio.addAsset(std::make_unique<PreciousMetal>(name, quantity, averagePrice, currentPrice));
        }
        else if (type == "Crypto") {
            portfolio.addAsset(std::make_unique<Crypto>(name, quantity, averagePrice, currentPrice));
        }
        };

    std::string cashLine = readStringBin(file);
    if (!cashLine.empty()) parseAsset(cashLine);

    size_t count = 0;
    if (file.read(reinterpret_cast<char*>(&count), sizeof(count))) {
        for (size_t i = 0; i < count; ++i) {
            std::string assetLine = readStringBin(file);
            if (!assetLine.empty()) parseAsset(assetLine);
        }
    }

    file.close();
    return true;
}