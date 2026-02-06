#ifndef CLOPE_H
#define CLOPE_H

#include "Cluster.h"
#include <vector>
#include <set>

class Clope
{
private:
    vector<Cluster> clusters{};
    vector<multiset<char>> transactions{};
    double repulsion;
    int transactionCounter;

public:
    Clope(double r);
    ~Clope();

    void initialize(const vector<multiset<char>> &data);
    void startClusterization();
    void finalize();

    vector<Cluster> getClusters() const { return clusters; }
    int getClusterCount() const { return clusters.size(); }

private:
    void doFirstIteration();
    bool iterateAllTransactions();
    double findMaxDelta(const multiset<char> &transaction, int &bestCluster, int exceptCluster = -1);

    double deltaAdd(const Cluster &cluster, const multiset<char> &transaction);
    double deltaRemove(const Cluster &cluster, const multiset<char> &transaction);
    double deltaNew(const multiset<char> &transaction);
};

#endif