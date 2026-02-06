#include "Clope.h"
#include <cmath>
#include <iostream>
#include <set>

using namespace std;

Clope::Clope(double r) : repulsion(r), transactionCounter(0)
{
    cout << "Clope initialized with repulsion = " << r << endl;
}
Clope::~Clope()
{
    cout << "Clope destroyed" << endl;
}
void Clope::initialize(const vector<multiset<char>> &data)
{
    transactions = data;
    transactionCounter = static_cast<int>(transactions.size());
    clusters.clear();
    cout << "Initialized with " << transactionCounter << " transactions" << endl;
}

void Clope::startClusterization()
{
    cout << "Starting clusterization..." << endl;
    doFirstIteration();

    int maxIterations = 10;
    int iteration = 0;

    while (iteration < maxIterations)
    {
        bool changed = iterateAllTransactions();
        cout << "Iteration " << iteration + 1 << ": "
             << (changed ? "changed" : "no change") << endl;
        if (!changed)
        {
            break;
        }
        iteration++;
    }

    finalize();
    cout << "Clusterization completed. Found " << clusters.size() << " clusters." << endl;
}

// Удаляем пустые кластеры
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
    if (transactions.empty())
    {
        return;
    }

    cout << "Performing first iteration..." << endl;

    // Создаем первый кластер с первой транзакцией
    Cluster firstCluster;
    multiset<char> firstTrans = transactions[0];
    string transStr(firstTrans.begin(), firstTrans.end());
    firstCluster.AddTransaction(transStr);
    clusters.push_back(firstCluster);

    // Обрабатываем остальные транзакции
    for (int i = 1; i < transactions.size(); i++)
    {
        int bestCluster = -1;
        double maxDelta = findMaxDelta(transactions[i], bestCluster);

        if (bestCluster == static_cast<int>(clusters.size()))
        {
            // Создаем новый кластер
            Cluster newCluster;
            string transStr(transactions[i].begin(), transactions[i].end());
            newCluster.AddTransaction(transStr);
            clusters.push_back(newCluster);
        }
        else if (bestCluster >= 0 && bestCluster < static_cast<int>(clusters.size()))
        {
            // Добавляем в существующий кластер
            string transStr(transactions[i].begin(), transactions[i].end());
            clusters[bestCluster].AddTransaction(transStr);
        }
    }
}

bool Clope::iterateAllTransactions()
{
    bool changed = false;

    // Вектор для хранения текущего распределения транзакций по кластерам
    vector<int> transactionToCluster(transactions.size(), -1);

    for (int i = 0; i < transactions.size(); i++)
    {

        for (int j = 0; j < clusters.size(); j++)
        {

            bool allItemsFound = true;
            for (char c : transactions[i])
            {
                if (!clusters[j].containsItem(c))
                {
                    allItemsFound = false;
                    break;
                }
            }
            if (allItemsFound)
            {
                transactionToCluster[i] = j;
                break;
            }
        }
    }
    // Перераспределяем транзакции
    for (int i = 0; i < transactions.size(); i++)
    {
        int currentCluster = transactionToCluster[i];
        if (currentCluster == -1)
        {
            continue;
        }

        // Удаляем транзакцию из текущего кластера
        string transStr(transactions[i].begin(), transactions[i].end());
        clusters[currentCluster].RemoveTransaction(transStr);

        // Находим лучший кластер для транзакции
        int bestCluster = -1;
        double maxDelta = findMaxDelta(transactions[i], bestCluster, currentCluster);

        if (bestCluster != currentCluster && bestCluster != -1)
        {
            changed = true;
            // Добавляем в новый кластер
            if (bestCluster == static_cast<int>(clusters.size()))
            {
                // Создаем новый кластер
                Cluster newCluster;
                newCluster.AddTransaction(transStr);
                clusters.push_back(newCluster);
                transactionToCluster[i] = static_cast<int>(clusters.size()) - 1;
            }
            else
            {
                clusters[bestCluster].AddTransaction(transStr);
                transactionToCluster[i] = bestCluster;
            }
        }
        else
        {
            // Возвращаем в исходный кластер
            clusters[currentCluster].AddTransaction(transStr);
        }
    }

    return changed;
}
double Clope::findMaxDelta(const multiset<char> &transaction, int &bestCluster, int exceptCluster)
{
    double maxDelta = -1e9;
    bestCluster = -1;

    // Проверяем существующие кластеры
    for (int i = 0; i < static_cast<int>(clusters.size()); i++)
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
        return deltaNew(transaction);
    }

    // Вычисляем новую ширину
    int newW = cluster.getWidth();

    // Проверяем, какие элементы уже есть в кластере
    const multiset<char> &clusterOcc = cluster.getOcc();
    for (char c : transaction)
    {
        if (clusterOcc.find(c) == clusterOcc.end())
        {
            newW++;
        }
    }

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
        return -cluster.getArea() * cluster.getCounter() / pow(cluster.getWidth(), repulsion);
    }

    // Упрощенный расчет
    int newW = cluster.getWidth();

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