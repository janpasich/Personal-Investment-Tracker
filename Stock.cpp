#include "Stock.h"

// --- CONSTRUCTOR ---
Stock::Stock(const std::string& t, double l, double s, double k, double d)
    : ticker(t), numberOfShares(l), averagePrice(s), currentPrice(k), dividendPerShare(d) {
}

// --- SETTERS (Specific to Stock) ---
void Stock::updateDividend(double newDividend) {
    dividendPerShare = newDividend;
}

// --- GETTERS (Specific to Stock, extracted directly by GUI when needed) ---
double Stock::getCurrentPrice() const {
    return currentPrice;
}

double Stock::getAveragePrice() const {
    return averagePrice;
}

double Stock::getNumberOfShares() const {
    return numberOfShares;
}

double Stock::calculateProfitLoss() const {
    return numberOfShares * (currentPrice - averagePrice);
}

// --- VIRTUAL FUNCTIONS IMPLEMENTATION ---

void Stock::setCurrentPrice(double newPrice) {
    // This is our only method through which the API pushes new data
    currentPrice = newPrice;
}

double Stock::calculateValue() const {
    return numberOfShares * currentPrice;
}

std::string Stock::getName() const {
    return ticker;
}

double Stock::calculateEstimatedDividend() const {
    return numberOfShares * dividendPerShare;
}

double Stock::getQuantity() const {
    return numberOfShares;
}

void Stock::addToPosition(double newQuantity, double transactionPrice) {
    double oldPositionValue = numberOfShares * averagePrice;
    double newPositionValue = newQuantity * transactionPrice;

    numberOfShares += newQuantity;
    averagePrice = (oldPositionValue + newPositionValue) / numberOfShares;
}

bool Stock::subtractFromPosition(double quantityToSell) {
    if (quantityToSell > numberOfShares) {
        return false;
    }

    numberOfShares -= quantityToSell;
    return true;
}

std::string Stock::formatToCSV() const {
    return "Stock;" + ticker + ";" +
        std::to_string(numberOfShares) + ";" +
        std::to_string(averagePrice) + ";" +
        std::to_string(currentPrice) + ";" +
        std::to_string(dividendPerShare);
}