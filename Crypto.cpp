#include "Crypto.h"

// --- CONSTRUCTOR ---
Crypto::Crypto(std::string t, double i, double s, double k)
    : ticker(t), tokenAmount(i), averagePurchasePrice(s), currentPrice(k) {
}

// --- GETTERS (Specific to Crypto) ---
double Crypto::getAveragePrice() const {
    return averagePurchasePrice;
}

double Crypto::getCurrentPrice() const {
    return currentPrice;
}

double Crypto::calculateProfitLoss() const {
    return tokenAmount * (currentPrice - averagePurchasePrice);
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

double Crypto::calculateEstimatedDividend() const {
    return 0; // Crypto does not pay dividends
}

double Crypto::getQuantity() const {
    return tokenAmount;
}

void Crypto::addToPosition(double newQuantity, double transactionPrice) {
    double oldPositionValue = tokenAmount * averagePurchasePrice;
    double newPositionValue = newQuantity * transactionPrice;

    tokenAmount += newQuantity;
    averagePurchasePrice = (oldPositionValue + newPositionValue) / tokenAmount;
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
    return "Crypto;" + ticker + ";" +
        std::to_string(tokenAmount) + ";" +
        std::to_string(averagePurchasePrice) + ";" +
        std::to_string(currentPrice) + ";0";
}