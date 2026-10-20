#pragma once
#include <string>
#include "Asset.h"

class Crypto : public Asset {
private:
    std::string ticker;                // API symbol (e.g., "BTC", "ETH")
    double tokenAmount;                // Quantity of owned coins/tokens
    double averagePrice;       // Average purchase price per token
    double currentPrice;               // Market price fetched by API

public:
    // Constructor
    Crypto(const std::string& t, double i, double s, double k);

    // --- API PREPARATION (Setters) ---
    virtual void setCurrentPrice(double newPrice) override;

    // --- GUI PREPARATION (Getters) ---
    double getCurrentPrice() const;
    double calculateProfitLoss() const;
    double getAveragePrice() const override;

    // --- VIRTUAL FUNCTIONS IMPLEMENTATION (from Asset contract) ---
    virtual double calculateValue() const override;
    virtual std::string getName() const override;
    virtual double getQuantity() const override;
    virtual void addToPosition(double newQuantity, double transactionPrice) override;
    virtual bool subtractFromPosition(double quantityToSell) override;
    virtual std::string formatToCSV() const override;
};