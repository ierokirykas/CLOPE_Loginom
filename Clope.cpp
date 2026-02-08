#include "Clope.h"

CLOPE::CLOPE(double repulsion, int noiseLimit) : r(repulsion), noiseLimit(noiseLimit) {}

// Инициализируем кластеры
void CLOPE::initClusters(const vector<vector<int>> &transactions)
{
    int numTransactions = transactions.size();
    ClusTruns.resize(numTransactions, -1);

    for (int i = 0; i < numTransactions; ++i)
    {
        const auto &trans = transactions[i];
        double maxDelta = -1000000;
        int bestCluster = -1;

        // Обновляем наилучшие значения
        for (size_t c = 0; c < clusters.size(); ++c)
        {
            double delta = clusters[c].deltaAdd(trans, r);
            if (delta > maxDelta)
            {
                maxDelta = delta;
                bestCluster = c;
            }
        }

        // Проверяем новый кластер
        Cluster newCluster;
        double deltaNew = newCluster.deltaAdd(trans, r);
        if (deltaNew > maxDelta)
        {
            clusters.push_back(newCluster);
            bestCluster = clusters.size() - 1;
        }

        // Добавляем транзакцию в выбранный кластер
        clusters[bestCluster].addTransaction(trans);
        ClusTruns[i] = bestCluster;
    }

    removeNoiseClusters();
};

// Следующая итерация
int CLOPE::nextStep(const vector<vector<int>> &transactions)
{
    int moves = 0;
    int numTransactions = transactions.size();

    for (int i = 0; i < numTransactions; ++i)
    {
        const auto &trans = transactions[i];
        int currentCluster = ClusTruns[i];

        // Удаляем транзакцию из текущего кластера
        clusters[currentCluster].removeTransaction(trans);

        // Ищем лучший кластер для транзакции
        double maxDelta = -1000000;
        int bestCluster = -1;

        for (size_t c = 0; c < clusters.size(); ++c)
        {
            if (clusters[c].count == 0)
                continue;
            double delta = clusters[c].deltaAdd(trans, r);
            if (delta > maxDelta)
            {
                maxDelta = delta;
                bestCluster = c;
            }
        }

        // Проверяем новый кластер
        Cluster newCluster;
        double deltaNew = newCluster.deltaAdd(trans, r);
        if (deltaNew > maxDelta)
        {
            clusters.push_back(newCluster);
            bestCluster = clusters.size() - 1;
        }

        if (bestCluster != currentCluster)
        {
            moves++;
        }

        // Добавляем транзакцию в новый кластер
        clusters[bestCluster].addTransaction(trans);
        ClusTruns[i] = bestCluster;
    }

    removeNoiseClusters();
    return moves;
};

// Убираем шумовые кластеры
void CLOPE::removeNoiseClusters()
{
    vector<Cluster> newClusters;
    vector<int> clusterMapping(clusters.size(), -1);

    for (size_t i = 0; i < clusters.size(); ++i)
    {
        if (clusters[i].count > noiseLimit)
        {
            clusterMapping[i] = newClusters.size();
            newClusters.push_back(clusters[i]);
        }
    }

    // Обновляем индексы кластеров для транзакций
    for (size_t i = 0; i < ClusTruns.size(); ++i)
    {
        int oldCluster = ClusTruns[i];
        if (oldCluster >= 0 && clusterMapping[oldCluster] != -1)
        {
            ClusTruns[i] = clusterMapping[oldCluster];
        }
        else
        {
            ClusTruns[i] = -1; // Транзакция помечена как шум
        }
    }

    clusters = move(newClusters);
};

// Запуск алгоритма до сходимости
void CLOPE::fit(const vector<vector<int>> &transactions, int maxIterations)
{
    initClusters(transactions);
    for (int iter = 0; iter < maxIterations; ++iter)
    {
        int moves = nextStep(transactions);
        cout << "Итерация " << iter + 1 << ": перемещений = " << moves << endl;
        if (moves == 0)
            break;
    }
};