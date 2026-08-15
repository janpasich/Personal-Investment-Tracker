#pragma once
#include <string>
#include "Asset.h"

class PreciousMetal : public Asset {
private:
    std::string ticker;              // API symbol (e.g., "XAG" for silver, "XAU" for gold)
    double weightOunces;             // Total weight in troy ounces (oz)
    double averagePricePerOunce;     // Purchase price per ounce (for profit/loss calculation)
    double currentPrice;             // Market price fetched by API

public:
    // Constructor
    PreciousMetal(std::string t, double w, double s, double k);

    // --- API PREPARATION (Setters) ---
    void updatePrice(double newPrice); // Note: Declared but not implemented in your original .cpp
    virtual void setCurrentPrice(double newPrice) override;

    // --- GUI PREPARATION (Getters) ---
    double getAveragePrice() const;
    double getWeight() const;
    double getCurrentPrice() const;
    double calculateProfitLoss() const;

    // --- VIRTUAL FUNCTIONS IMPLEMENTATION (from Asset contract) ---
    virtual double calculateValue() const override;
    virtual std::string getName() const override;
    virtual double calculateEstimatedDividend() const override;
    virtual double getQuantity() const override;
    virtual void addToPosition(double newQuantity, double transactionPrice) override;
    virtual bool subtractFromPosition(double quantityToSell) override;
    virtual std::string formatToCSV() const override;
};