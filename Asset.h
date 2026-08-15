#pragma once
#include <string>

class Asset {
public:
    virtual ~Asset() = default;
    virtual double calculateValue() const = 0;
    virtual std::string getName() const = 0;
    virtual double calculateEstimatedDividend() const = 0;
    virtual double getQuantity() const = 0;
    virtual void addToPosition(double newQuantity, double transactionPrice) = 0;
    virtual bool subtractFromPosition(double quantityToSell) = 0;
    virtual std::string formatToCSV() const = 0;
    virtual void setCurrentPrice(double newPrice) {}
};