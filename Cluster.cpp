#include "Cluster.h"
#include <algorithm>
#include <cmath>

using namespace std;

Cluster::Cluster() = default;

Cluster::Cluster(int S, int W, int N) : S(S), W(W), N(N) {}

Cluster::~Cluster() = default;

bool Cluster::containsItem(char item) const
{
    return Occ.find(item) != Occ.end();
}

// Добавляем элементы транзакции в множество
void Cluster::AddTransaction(const string &t)
{

    for (char c : t)
    {
        Occ.insert(c);
    }

    S += static_cast<int>(t.length());
    N += 1;
    W = static_cast<int>(Occ.size());
}

// Удаляем элементы
void Cluster::RemoveTransaction(const string &t)
{

    for (char c : t)
    {
        auto it = Occ.find(c);
        if (it != Occ.end())
        {
            Occ.erase(it);
        }
    }

    // Обновляем метрики
    S -= static_cast<int>(t.length());
    N -= 1;
    if (Occ.empty())
    {
        W = 0;
    }
    else
    {
        W = static_cast<int>(Occ.size());
    }
}

// Вычисляем новое значение при добавление транзакции
double Cluster::AddCost(const string &t, double repulsion) const
{
    if (W == 0)
    {
        // Пустой кластер
        return static_cast<double>(t.length()) / pow(static_cast<double>(t.length()), repulsion);
    }

    // Вычисляем новую ширину
    int newW = W;
    for (char c : t)
    {
        if (Occ.find(c) == Occ.end())
        {
            newW++;
        }
    }

    if (newW == 0)
        return 0;

    double newValue = (S + static_cast<int>(t.length())) * (N + 1) / pow(newW, repulsion);
    double oldValue = S * N / pow(W, repulsion);

    return newValue - oldValue;
}

// То же при удалении
double Cluster::RemoveCost(const string &t, double repulsion) const
{

    if (N <= 1)
    {
        return -S * N / pow(W, repulsion);
    }

    int newW = W;
    if (newW == 0)
        return 0;

    double newValue = (S - static_cast<int>(t.length())) * (N - 1) / pow(newW, repulsion);
    double oldValue = S * N / pow(W, repulsion);

    return newValue - oldValue;
}