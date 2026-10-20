#pragma once
#include <string>
#include "Asset.h"

class Stock : public Asset {
private:
    std::string ticker;
    double numberOfShares;
    double averagePrice;
    double currentPrice;
    double dividendPerShare;

public:
    // Constructor
    Stock(const std::string& t, double l, double s, double k, double d);

    // --- 1. API PREPARATION (Setters) ---
    void updateDividend(double newDividend);
    virtual void setCurrentPrice(double newPrice) override;

    // --- 2. GUI PREPARATION (Getters) ---
    double getCurrentPrice() const;
    double getAveragePrice() const override;
    double getNumberOfShares() const;
    double calculateProfitLoss() const;

    // --- VIRTUAL FUNCTIONS IMPLEMENTATION (from Asset.h) ---
    virtual double calculateValue() const override;
    virtual std::string getName() const override;
    virtual double calculateEstimatedDividend() const override;
    virtual double getQuantity() const override;
    virtual void addToPosition(double newQuantity, double transactionPrice) override;
    virtual bool subtractFromPosition(double quantityToSell) override;
    virtual std::string formatToCSV() const override;
};