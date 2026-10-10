#include "PreciousMetal.h"
#include <sstream>

// --- CONSTRUCTOR ---
PreciousMetal::PreciousMetal(const std::string& t, double w, double s, double k)
    : ticker(t), weightOunces(w), averagePrice(s), currentPrice(k) {
}

// --- GETTERS (Specific to PreciousMetal) ---
double PreciousMetal::getAveragePrice() const {
    return averagePrice;
}

double PreciousMetal::getWeight() const {
    return weightOunces;
}

double PreciousMetal::getCurrentPrice() const {
    return currentPrice;
}

double PreciousMetal::calculateProfitLoss() const {
    return weightOunces * (currentPrice - averagePrice);
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
    double oldPositionValue = weightOunces * averagePrice;
    double newPositionValue = newQuantity * transactionPrice;

    weightOunces += newQuantity;
    averagePrice = (oldPositionValue + newPositionValue) / weightOunces;
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
    std::ostringstream oss;
    oss << "PreciousMetal;"
        << ticker << ";"
        << weightOunces << ";"
        << averagePrice << ";"
        << currentPrice << ";0";

    return oss.str();
}