#include <Cluster.h>
#include <iostream>
#include <cmath>

double r = 1.6;

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
double Cluster::AddCost(Cluster C, string t)
{
    int S = C.S;
    int N = C.N;
    int W = C.W;

    int S_new = S + t.length();
    int N_new = N + 1;
    int W_new = W;
    set<int> tmp;

    for (int i = 0; i < t.length(); i++)
    {
        if (C.Occ.count(t[i]) == 0 && tmp.count(t[i]) == 0)
        {
            W_new = W_new + 1;
        }
        tmp.insert(t[i]);
    }

    return (S_new * N_new) / (pow(W_new, r)) - ((S == 0) ? 0 : (S * N) / (pow(W, r)));
}

double Cluster::RemoveCost(Cluster C, string t)
{
    int S = C.S;
    int N = C.N;
    int W = C.W;

    int S_new = S - t.length();
    int N_new = N - 1;
    int W_new = W;

    for (int i = 0; i < t.length(); i++)
    {
        if (C.Occ.count(t[i]) == 1)
        {
            W_new = W_new - 1;
        }
    }

    return ((W_new == 0) ? 0 : (S_new * N_new) / (pow(W_new, r))) - (S * N) / (pow(W, r));
}

bool Cluster::containsItem(char item) const
{
    return Occ.find(item) != Occ.end();
}
