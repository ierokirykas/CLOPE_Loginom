#include "Cluster.h"

// Добавляем транзакцию в кластер
void Cluster::addTransaction(const vector<int> &transaction)
{
    for (int item : transaction)
    {
        if (transactions.find(item) == transactions.end())
        {
            transactions[item] = 1;
            width += 1.0;
        }
        else
        {
            transactions[item] += 1;
        }
        area += 1.0;
    }
    count++;
}

// Удаляем транзакцию из кластера
void Cluster::removeTransaction(const vector<int> &transaction)
{
    for (int item : transaction)
    {
        auto it = transactions.find(item);
        if (it != transactions.end())
        {
            it->second -= 1;
            area -= 1.0;
            if (it->second == 0)
            {
                transactions.erase(it);
                width -= 1.0;
            }
        }
    }
    count--;
}

// Высчитываем целевую функцию кластера
double Cluster::gradient(double r) const
{
    if (width == 0.0)
        return 0.0;
    return (area * count) / std::pow(width, r);
}

// Вычисляем изменение градиента
double Cluster::deltaAdd(const vector<int> &transaction, double r) const
{
    double newArea = area + transaction.size();
    double newWidth = width;
    for (int item : transaction)
    {
        if (transactions.find(item) == transactions.end())
        {
            newWidth += 1.0;
        }
    }
    if (newWidth < 1e-9)
        return -1e9; // Очень маленькое значение вместо 0

    double newGradient = (newArea * (count + 1)) / std::pow(newWidth, r);
    double oldGradient = (width == 0.0) ? 0.0 : (area * count) / std::pow(width, r);
    return newGradient - oldGradient;
}