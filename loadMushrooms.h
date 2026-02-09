#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class DataLoader
{
private:
    unordered_map<string, int> featureToId;
    unordered_map<int, string> idToFeature;
    int nextId = 1;

public:
    DataLoader() = default;

    vector<vector<int>> loadMushrooms(const string &filename,
                                      vector<char> &labels);

    vector<vector<int>> loadMushroomData(const string &filename,
                                         vector<char> &labels,
                                         bool removeMissing = true);

    int getFeatureId(const string &feature);

    string getFeatureValue(int id) const;

private:
    vector<string> split(const string &str, char delimiter);
};