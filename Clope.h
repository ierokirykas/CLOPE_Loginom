#ifndef CLOPE_H
#define CLOPE_H

#include "Cluster.h"
#include <vector>
#include <set>

class Clope
{
private:
    multiset<Cluster> clusters{};
    multiset<char> transactions{};
    set<int> noiseClusters{};
    int N_tran{0};
    int N_iter{0};
    int max_cluster_number{0};

public:
    Clope(/* args */);
    ~Clope();
    double deltaAdd(const Cluster &cluster, const multiset<char> &transaction, double r);
    double deltaRemove(const Cluster &cluster, const multiset<char> &transaction, double r);

    void addCluster(const Cluster &cluster);
    void addTransaction(const multiset<char> &transaction);
};

#endif