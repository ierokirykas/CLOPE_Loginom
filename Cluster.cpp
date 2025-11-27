#include <Cluster.h>
#include <iostream>

Cluster::Cluster() = default; // Конструктор по умолчанию (Все значения = 0)
Cluster::Cluster(int s, int w, int n) : S(s), W(w), N(n) {}
Cluster::~Cluster()
{
    cout << "Кластер удалён" << endl;
};

void Cluster::AddTransaction(Cluster &C, string t)
{
    C.S = C.S + t.length();
    C.N = C.N + 1;
    for (int i = 0; i < t.length(); i++)
    {
        if (C.Occ.count(t[i]) == 0)
        {
            C.W = C.W + 1;
        }
        C.Occ.insert(t[i]);
    }
};

void Cluster::RemoveTransaction(Cluster &C, string t)
{
    C.S = C.S - t.length();
    C.N = C.N - 1;
    for (int i = 0; i < t.length(); i++)
    {
        if (C.Occ.count(t[i]) == 1)
        {
            C.W = C.W - 1;
        }
        C.Occ.erase(C.Occ.find(t[i]));
    }
}

bool Cluster::containsItem(char item) const
{
    return Occ.find(item) != Occ.end();
}
