#pragma once
#include <string>
#include "Asset.h"

class Crypto : public Asset {
private:
    std::string ticker;                // API symbol (e.g., "BTC", "ETH")
    double tokenAmount;                // Quantity of owned coins/tokens
    double averagePurchasePrice;       // Average purchase price per token
    double currentPrice;               // Market price fetched by API

public:
    // Constructor
    Crypto(std::string t, double i, double s, double k);

    // --- API PREPARATION (Setters) ---
    void updatePrice(double newPrice); // Note: Declared but not implemented in the .cpp
    virtual void setCurrentPrice(double newPrice) override;

    // --- GUI PREPARATION (Getters) ---
    double getAveragePrice() const;
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