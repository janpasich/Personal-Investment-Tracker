#pragma once
#include <string>
#include <fstream> // Dodane dla obs³ugi std::ofstream i std::ifstream
#include "Portfolio.h"
#include "Cash.h"

class SaveManager {
private:
    static void writeStringBin(std::ofstream& file, const std::string& str);
    static std::string readStringBin(std::ifstream& file);

public:
    static bool savePortfolioBinary(const Portfolio& portfolio, Cash* cash, const std::string& filename);
    static bool loadPortfolioBinary(Portfolio& portfolio, Cash* cash, const std::string& filename);
};