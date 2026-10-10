#pragma once
#include <string>
#include <fstream>
#include "Portfolio.h"

class SaveManager {
private:
    static void writeStringBin(std::ofstream& file, const std::string& str);
    static std::string readStringBin(std::ifstream& file);

public:
    static bool savePortfolioBinary(const Portfolio& portfolio, const std::string& filename);
    static bool loadPortfolioBinary(Portfolio& portfolio, const std::string& filename);
};