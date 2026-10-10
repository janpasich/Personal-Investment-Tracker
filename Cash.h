#pragma once
#include "Asset.h"
#include <string>

class Cash : public Asset {
private:
    double availableFunds; // Stores the amount of available cash

public:
    // Constructor setting the initial capital
    Cash(double initialCapital);


    // --- Virtual functions implementation from Asset contract ---
    virtual double calculateValue() const override;
    virtual std::string getName() const override;
    virtual double getQuantity() const override;
    virtual void addToPosition(double newQuantity, double transactionPrice) override;
    virtual bool subtractFromPosition(double amountToSubtract) override;
    virtual std::string formatToCSV() const override;
    virtual double getAveragePrice() const override;
};