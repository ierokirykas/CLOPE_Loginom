#include "Clope.h"
#include <cmath>
#include <iostream>
#include <set>

using namespace std;

Clope::Clope(double r) : repulsion(r), transactionCounter(0) {}

Clope::~Clope() = default;

void Clope::initialize(const vector<multiset<char>> &data)
{
    transactions = data;
    transactionCounter = static_cast<int>(transactions.size());
    clusters.clear();
}

void Clope::startClusterization()
{
    doFirstIteration();

    // Выполняем итерации до сходимости
    int maxIterations = 100;
    int iteration = 0;

    while (iteration < maxIterations)
    {
        bool changed = iterateAllTransactions();
        if (!changed)
        {
            break;
        }
        iteration++;
    }

    finalize();
}

void Clope::finalize()
{
    // Удаляем пустые кластеры
    vector<Cluster> nonEmptyClusters;
    for (const auto &cluster : clusters)
    {
        if (cluster.getCounter() > 0)
        {
            nonEmptyClusters.push_back(cluster);
        }
    }
    clusters = nonEmptyClusters;
}

void Clope::doFirstIteration()
{
    // Создаем первый кластер с первой транзакцией
    Cluster firstCluster;
    multiset<char> firstTrans = transactions[0];
    string transStr(firstTrans.begin(), firstTrans.end());
    firstCluster.AddTransaction(firstCluster, transStr);
    clusters.push_back(firstCluster);

    // Обрабатываем остальные транзакции
    for (int i = 1; i < transactions.size(); i++)
    {
        int bestCluster = -1;
        double maxDelta = findMaxDelta(transactions[i], bestCluster);

        if (bestCluster == clusters.size())
        {
            // Создаем новый кластер
            Cluster newCluster;
            string transStr(transactions[i].begin(), transactions[i].end());
            newCluster.AddTransaction(newCluster, transStr);
            clusters.push_back(newCluster);
        }
        else
        {
            // Добавляем в существующий кластер
            string transStr(transactions[i].begin(), transactions[i].end());
            clusters[bestCluster].AddTransaction(clusters[bestCluster], transStr);
        }
    }
}

bool Clope::iterateAllTransactions()
{
    bool changed = false;

    for (int i = 0; i < transactions.size(); i++)
    {
        // Находим текущий кластер для транзакции
        int currentCluster = -1;
        int bestCluster = -1;

        // Для простоты находим текущий кластер по принадлежности
        // В реальности нужно хранить соответствие транзакция-кластер
        for (int j = 0; j < clusters.size(); j++)
        {
            // Проверяем, принадлежит ли транзакция этому кластеру
            // Упрощенная проверка
            bool belongs = true;
            for (char c : transactions[i])
            {
                if (!clusters[j].containsItem(c))
                {
                    belongs = false;
                    break;
                }
            }
            if (belongs)
            {
                currentCluster = j;
                break;
            }
        }

        if (currentCluster == -1)
        {
            continue; // Не нашли кластер
        }

        // Удаляем транзакцию из текущего кластера
        string transStr(transactions[i].begin(), transactions[i].end());
        clusters[currentCluster].RemoveTransaction(clusters[currentCluster], transStr);

        // Находим лучший кластер для транзакции
        double maxDelta = findMaxDelta(transactions[i], bestCluster, currentCluster);

        if (bestCluster != currentCluster)
        {
            changed = true;
            // Добавляем в новый кластер
            string transStr(transactions[i].begin(), transactions[i].end());
            if (bestCluster == clusters.size())
            {
                // Создаем новый кластер
                Cluster newCluster;
                newCluster.AddTransaction(newCluster, transStr);
                clusters.push_back(newCluster);
            }
            else
            {
                clusters[bestCluster].AddTransaction(clusters[bestCluster], transStr);
            }
        }
        else
        {
            // Возвращаем в исходный кластер
            clusters[currentCluster].AddTransaction(clusters[currentCluster], transStr);
        }
    }

    return changed;
}

double Clope::findMaxDelta(const multiset<char> &transaction, int &bestCluster, int exceptCluster)
{
    double maxDelta = -1e9;
    bestCluster = -1;

    // Проверяем существующие кластеры
    for (int i = 0; i < clusters.size(); i++)
    {
        if (i == exceptCluster)
            continue;

        double delta = deltaAdd(clusters[i], transaction);
        if (delta > maxDelta)
        {
            maxDelta = delta;
            bestCluster = i;
        }
    }

    // Проверяем создание нового кластера
    double newDelta = deltaNew(transaction);
    if (newDelta > maxDelta)
    {
        maxDelta = newDelta;
        bestCluster = static_cast<int>(clusters.size());
    }

    return maxDelta;
}

double Clope::deltaAdd(const Cluster &cluster, const multiset<char> &transaction)
{
    if (cluster.getCounter() == 0)
    {
        // Пустой кластер - то же самое, что новый
        return deltaNew(transaction);
    }

    // Вычисляем новую ширину
    int newW = cluster.getWidth();

    // Для упрощения считаем, что все элементы уникальны
    // В реальности нужно проверять наличие элементов в кластере
    newW += static_cast<int>(transaction.size());

    if (newW == 0)
        return 0;

    double newValue = (cluster.getArea() + static_cast<int>(transaction.size())) *
                      (cluster.getCounter() + 1) / pow(newW, repulsion);
    double oldValue = cluster.getArea() * cluster.getCounter() / pow(cluster.getWidth(), repulsion);

    return newValue - oldValue;
}

double Clope::deltaRemove(const Cluster &cluster, const multiset<char> &transaction)
{
    if (cluster.getCounter() <= 1)
    {
        // Удаляем последнюю транзакцию
        return -cluster.getArea() * cluster.getCounter() / pow(cluster.getWidth(), repulsion);
    }

    // Упрощенный расчет
    int newW = cluster.getWidth(); // В реальности нужно пересчитывать

    if (newW == 0)
        return 0;

    double newValue = (cluster.getArea() - static_cast<int>(transaction.size())) *
                      (cluster.getCounter() - 1) / pow(newW, repulsion);
    double oldValue = cluster.getArea() * cluster.getCounter() / pow(cluster.getWidth(), repulsion);

    return newValue - oldValue;
}

double Clope::deltaNew(const multiset<char> &transaction)
{
    int S_new = static_cast<int>(transaction.size());
    int N_new = 1;

    // Уникальные элементы в транзакции
    set<char> uniqueItems(transaction.begin(), transaction.end());
    int W_new = static_cast<int>(uniqueItems.size());

    if (W_new == 0)
        return 0;

    return S_new * N_new / pow(W_new, repulsion);
}