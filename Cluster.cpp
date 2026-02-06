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

void Cluster::AddTransaction(Cluster &C, string t)
{
    // Добавляем элементы транзакции в множество
    for (char c : t)
    {
        Occ.insert(c);
    }

    // Обновляем метрики
    S += static_cast<int>(t.length());
    N += 1;
    W = static_cast<int>(Occ.size());
}

void Cluster::RemoveTransaction(Cluster &C, string t)
{
    // Удаляем элементы транзакции из множества
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

double Cluster::AddCost(Cluster C, string t)
{
    // Вычисляем дельту при добавлении транзакции
    if (W == 0)
    {
        // Пустой кластер
        return static_cast<double>(t.length()) / pow(static_cast<double>(t.length()), 2.0);
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

    double newValue = (S + static_cast<int>(t.length())) * (N + 1) / pow(newW, 2.0);
    double oldValue = S * N / pow(W, 2.0);

    return newValue - oldValue;
}

double Cluster::RemoveCost(Cluster C, string t)
{
    // Вычисляем дельту при удалении транзакции
    if (N <= 1)
    {
        // Если удаляем последнюю транзакцию, кластер станет пустым
        return -S * N / pow(W, 2.0);
    }

    // Вычисляем новую ширину (упрощенно - предполагаем, что все элементы остаются)
    // В реальности нужно пересчитывать уникальные элементы
    int newW = W; // Упрощение - в реальности нужно считать уникальные элементы

    if (newW == 0)
        return 0;

    double newValue = (S - static_cast<int>(t.length())) * (N - 1) / pow(newW, 2.0);
    double oldValue = S * N / pow(W, 2.0);

    return newValue - oldValue;
}