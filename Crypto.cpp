#include "Crypto.h"
#include <sstream>

// --- CONSTRUCTOR ---
Crypto::Crypto(const std::string& t, double i, double s, double k)
    : ticker(t), tokenAmount(i), averagePrice(s), currentPrice(k) {
}

// --- GETTERS (Specific to Crypto) ---
double Crypto::getAveragePrice() const {
    return averagePrice;
}

double Crypto::getCurrentPrice() const {
    return currentPrice;
}

double Crypto::calculateProfitLoss() const {
    return tokenAmount * (currentPrice - averagePrice);
}

// --- VIRTUAL FUNCTIONS IMPLEMENTATION (Contract from Asset class) ---

void Crypto::setCurrentPrice(double newPrice) {
    // Function called by API to update the price
    currentPrice = newPrice;
}

double Crypto::calculateValue() const {
    return tokenAmount * currentPrice;
}

std::string Crypto::getName() const {
    return ticker;
}


double Crypto::getQuantity() const {
    return tokenAmount;
}

void Crypto::addToPosition(double newQuantity, double transactionPrice) {
    double oldPositionValue = tokenAmount * averagePrice;
    double newPositionValue = newQuantity * transactionPrice;

    tokenAmount += newQuantity;
    averagePrice = (oldPositionValue + newPositionValue) / tokenAmount;
}

bool Crypto::subtractFromPosition(double quantityToSell) {
    // Guard: cannot sell more tokens than owned
    if (quantityToSell > tokenAmount) {
        return false;
    }

    tokenAmount -= quantityToSell;
    return true;
}

std::string Crypto::formatToCSV() const {
    std::ostringstream oss;
    oss << "Crypto;"
        << ticker << ";"
        << tokenAmount << ";"
        << averagePrice << ";"
        << currentPrice << ";0";

    return oss.str();
}