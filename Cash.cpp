#include "Cash.h"
#include <sstream>

// --- CONSTRUCTOR ---
Cash::Cash(double initialCapital) : funds(initialCapital) {
}


// --- VIRTUAL FUNCTIONS IMPLEMENTATION ---

double Cash::calculateValue() const {
    return funds;
}

double Cash::getAveragePrice() const {
    return 1.0;
}

std::string Cash::getName() const {
    return "PLN";
}

double Cash::getQuantity() const {
    return funds;
}

void Cash::addToPosition(double newQuantity, double /*transactionPrice*/) {
    funds += newQuantity;
}

bool Cash::subtractFromPosition(double amountToSubtract) {
    // Guard: cannot spend more cash than is available in the account
    if (amountToSubtract > funds) {
        return false;
    }

    funds -= amountToSubtract;
    return true;
}

std::string Cash::formatToCSV() const {
    std::ostringstream oss;
    oss << "Cash;PLN;"
        << funds << ";1.0;1.0;0";

    return oss.str();
}