#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>

class DataLoader
{
private:
    std::unordered_map<std::string, int> featureToId;
    std::unordered_map<int, std::string> idToFeature;
    int nextId = 1;

public:
    DataLoader() = default;

    // Загрузить данные из файла
    std::vector<std::vector<int>> loadMushrooms(const std::string &filename,
                                                std::vector<char> &labels);

    // Преобразовать признак в ID
    int getFeatureId(const std::string &feature);

    // Получить исходное значение по ID
    std::string getFeatureValue(int id) const;

    // Загрузить данные для грибов (без первого признака)
    std::vector<std::vector<int>> loadMushroomData(const std::string &filename,
                                                   std::vector<char> &labels,
                                                   bool removeMissing = true);

private:
    // Утилита: разделить строку по разделителю
    std::vector<std::string> split(const std::string &str, char delimiter);
};