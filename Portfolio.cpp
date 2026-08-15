#include "Portfolio.h"

const std::vector<std::unique_ptr<Asset>>& Portfolio::getAssets() const {
    return assets;
}

double Portfolio::calculateTotalValue() const {
    double sum = 0.0;
    for (const auto& asset : assets) {
        sum += asset->calculateValue();
    }
    return sum;
}

double Portfolio::calculateTotalDividend() const {
    double totalDividend = 0.0;
    for (const auto& asset : assets) {
        totalDividend += asset->calculateEstimatedDividend();
    }
    return totalDividend;
}

void Portfolio::addAsset(std::unique_ptr<Asset> newAsset) {
    // std::move "moves" the ownership of the pointer to the vector
    assets.push_back(std::move(newAsset));
}

bool Portfolio::sellAsset(int index, double quantityToSell, double pricePerUnit) {
    if (index < 0 || index >= static_cast<int>(assets.size())) {
        return false;
    }

    if (assets[index]->subtractFromPosition(quantityToSell) == false) {
        return false;
    }

    // If the quantity drops to zero (or very close to it due to floating-point precision), we remove the asset
    if (assets[index]->getQuantity() <= 0.0001) {
        assets.erase(assets.begin() + index);
    }

    return true;
}

bool Portfolio::buyAsset(std::unique_ptr<Asset> newAsset) {
    double totalPrice = newAsset->calculateValue();

    for (auto& asset : assets) {
        // If we already have this asset, we average down/up the position instead of adding a new row
        if (asset->getName() == newAsset->getName()) {
            double pricePerUnit = totalPrice / newAsset->getQuantity();
            asset->addToPosition(newAsset->getQuantity(), pricePerUnit);

            return true;
        }
    }

    // If we don't have it, we add it as a completely new position
    assets.push_back(std::move(newAsset));
    return true;
}