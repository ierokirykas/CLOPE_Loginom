#include "Cluster.h"

#include <vector>
#include <iostream>
#include <limits>
class CLOPE
{
private:
    vector<Cluster> clusters;
    vector<int> ClusTruns; // Индекс кластера для каждой транзакции
    double r;              // Параметр отталкивания
    int noiseLimit;        // Порог для удаления шумовых кластеров

public:
    CLOPE(double repulsion = 2.0, int noiseLimit = 5);

    void initClusters(const vector<vector<int>> &transactions);
    int nextStep(const vector<vector<int>> &transactions);
    void removeNoiseClusters();
    void fit(const vector<vector<int>> &transactions, int maxIterations = 100);

    const std::vector<int> &getTransactions() const { return ClusTruns; }
    size_t getNumClusters() const { return clusters.size(); }

    void printClustersInfo() const;
};
