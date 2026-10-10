#include "Cash.h"

// --- CONSTRUCTOR ---
Cash::Cash(double initialCapital) : availableFunds(initialCapital) {
}


// --- VIRTUAL FUNCTIONS IMPLEMENTATION ---

double Cash::calculateValue() const {
    return availableFunds;
}

double Cash::getAveragePrice() const {
    return 1.0;
}

std::string Cash::getName() const {
    return "PLN";
}

double Cash::getQuantity() const {
    return availableFunds;
}

void Cash::addToPosition(double newQuantity, double /*transactionPrice*/) {
    availableFunds += newQuantity;
}

bool Cash::subtractFromPosition(double amountToSubtract) {
    // Guard: cannot spend more cash than is available in the account
    if (amountToSubtract > availableFunds) {
        return false;
    }

    availableFunds -= amountToSubtract;
    return true;
}

std::string Cash::formatToCSV() const {
    // Cash has a 1:1 exchange rate, so price parameters are 1.0
    return "Cash;PLN;" + std::to_string(availableFunds) + ";1.0;1.0;0";
}