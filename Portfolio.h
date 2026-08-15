#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Asset.h"

class Portfolio {
private:
    std::vector<std::unique_ptr<Asset>> assets;

public:
    ~Portfolio() = default;

    const std::vector<std::unique_ptr<Asset>>& getAssets() const;

    double calculateTotalValue() const;
    double calculateTotalDividend() const;

    // We pass the pointer by value using std::move (taking ownership)
    void addAsset(std::unique_ptr<Asset> newAsset);
    bool buyAsset(std::unique_ptr<Asset> newAsset);
    bool sellAsset(int index, double quantityToSell, double pricePerUnit);
};