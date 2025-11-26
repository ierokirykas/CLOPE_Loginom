#include <Cluster.h>
#include <iostream>

Cluster::Cluster() = default; // Конструктор по умолчанию (Все значения = 0)
Cluster::Cluster(int s, int w, int n) : S(s), W(w), N(n) {}
Cluster::~Cluster()
{
    cout << "Кластер удалён" << endl;
};
