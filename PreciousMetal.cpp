#include "PreciousMetal.h"

// --- CONSTRUCTOR ---
PreciousMetal::PreciousMetal(std::string t, double w, double s, double k)
    : ticker(t), weightOunces(w), averagePricePerOunce(s), currentPrice(k) {
}

// --- GETTERS (Specific to PreciousMetal) ---
double PreciousMetal::getAveragePrice() const {
    return averagePricePerOunce;
}

double PreciousMetal::getWeight() const {
    return weightOunces;
}

double PreciousMetal::getCurrentPrice() const {
    return currentPrice;
}

double PreciousMetal::calculateProfitLoss() const {
    return weightOunces * (currentPrice - averagePricePerOunce);
}

// --- VIRTUAL FUNCTIONS IMPLEMENTATION ---

void PreciousMetal::setCurrentPrice(double newPrice) {
    // API uses exclusively this method
    currentPrice = newPrice;
}

double PreciousMetal::calculateValue() const {
    return weightOunces * currentPrice;
}

std::string PreciousMetal::getName() const {
    return ticker;
}

double PreciousMetal::calculateEstimatedDividend() const {
    return 0; // Precious metals do not pay dividends
}

double PreciousMetal::getQuantity() const {
    return weightOunces;
}

void PreciousMetal::addToPosition(double newQuantity, double transactionPrice) {
    double oldPositionValue = weightOunces * averagePricePerOunce;
    double newPositionValue = newQuantity * transactionPrice;

    weightOunces += newQuantity;
    averagePricePerOunce = (oldPositionValue + newPositionValue) / weightOunces;
}

bool PreciousMetal::subtractFromPosition(double quantityToSell) {
    // Guard: cannot sell more ounces than owned
    if (quantityToSell > weightOunces) {
        return false;
    }

    weightOunces -= quantityToSell; // Subtract ounces (average purchase price remains unchanged)
    return true;
}

std::string PreciousMetal::formatToCSV() const {
    // Precious metal has no dividend, so we append 0 at the end
    return "PreciousMetal;" + ticker + ";" +
        std::to_string(weightOunces) + ";" +
        std::to_string(averagePricePerOunce) + ";" +
        std::to_string(currentPrice) + ";0";
}