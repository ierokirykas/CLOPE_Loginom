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
    const multiset<char> &getOcc() const { return Occ; }
    bool containsItem(char item) const;

    void AddTransaction(const string &t);
    void RemoveTransaction(const string &t);
    double AddCost(const string &t, double repulsion) const;
    double RemoveCost(const string &t, double repulsion) const;
};

#endif