#ifndef CLUSTER_H
#define CLUSTER_H

#include <set>
#include <string>
using namespace std;

class Cluster
{
private:
    multiset<char> Occ{}; // Множество объектов кластера
    int S{0};             // Площадь кластера
    int W{0};             // Ширина кластера
    int N{0};             // Кол-во вхождений (транзакций)

public:
    Cluster(); // Конструктор
    Cluster(int S, int W, int N);
    ~Cluster(); // Деструктор

    int getArea() const { return S; };
    int getWidth() const { return W; };
    int getCounter() const { return N; };
    bool containsItem(char item) const;

    void AddTransaction(Cluster &C, string t);
    void RemoveTransaction(Cluster &C, string t);
    double AddCost(Cluster C, string t);
    double RemoveCost(Cluster C, string t);
};

#endif