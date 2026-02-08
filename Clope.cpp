#include "Clope.h"
#include <algorithm>
#include <cassert>

CLOPE::CLOPE(double repulsion, int noiseLimit) : r(repulsion), noiseLimit(noiseLimit) {}

// Инициализируем кластеры
void CLOPE::initClusters(const vector<vector<int>> &transactions)
{
    int numTransactions = static_cast<int>(transactions.size());
    ClusTruns.resize(numTransactions, -1);
    clusters.clear();

    for (int i = 0; i < numTransactions; ++i)
    {
        const auto &trans = transactions[i];
        double maxDelta = -1000000;
        int bestCluster = -1;

        // Проверяем существующие кластеры
        for (size_t c = 0; c < clusters.size(); ++c)
        {
            if (!clusters[c].isEmpty())
            {
                double delta = clusters[c].deltaAdd(trans, r);
                if (delta > maxDelta)
                {
                    maxDelta = delta;
                    bestCluster = static_cast<int>(c);
                }
            }
        }

        // Проверяем новый кластер
        Cluster newCluster;
        double deltaNew = newCluster.deltaAdd(trans, r);
        if (deltaNew > maxDelta)
        {
            clusters.push_back(newCluster);
            clusters.back().addTransaction(trans);
            bestCluster = static_cast<int>(clusters.size() - 1);
        }
        else if (bestCluster != -1)
        {
            // Добавляем в существующий кластер
            clusters[bestCluster].addTransaction(trans);
        }
        else
        {
            // Создаем новый кластер, если все существующие пустые
            clusters.push_back(newCluster);
            clusters.back().addTransaction(trans);
            bestCluster = static_cast<int>(clusters.size() - 1);
        }
        // Добавляем транзакцию в выбранный кластер
        ClusTruns[i] = bestCluster;
    }

    removeNoiseClusters();
};

// Следующая итерация
int CLOPE::nextStep(const vector<vector<int>> &transactions)
{
    int moves = 0;
    int numTransactions = static_cast<int>(transactions.size());

    for (int i = 0; i < numTransactions; ++i)
    {
        const auto &trans = transactions[i];
        int currentCluster = ClusTruns[i];

        // Пропускаем транзакции, помеченные как шум (-1)
        if (currentCluster == -1)
            continue;

        // Проверяем, что индекс кластера корректен
        if (static_cast<size_t>(currentCluster) >= clusters.size())
        {
            ClusTruns[i] = -1;
            continue;
        }

        // Удаляем транзакцию из текущего кластера
        clusters[currentCluster].removeTransaction(trans);

        // Ищем лучший кластер для транзакции
        double maxDelta = -numeric_limits<double>::max();
        int bestCluster = -1;

        for (size_t c = 0; c < clusters.size(); ++c)
        {
            if (!clusters[c].isEmpty())
            {
                double delta = clusters[c].deltaAdd(trans, r);
                if (delta > maxDelta)
                {
                    maxDelta = delta;
                    bestCluster = static_cast<int>(c);
                }
            }
        }

        // Проверяем новый кластер
        Cluster newCluster;
        double deltaNew = newCluster.deltaAdd(trans, r);
        if (deltaNew > maxDelta)
        {
            clusters.push_back(newCluster);
            clusters.back().addTransaction(trans);
            bestCluster = static_cast<int>(clusters.size() - 1);
        }
        else if (bestCluster != -1)
        {
            // Добавляем в существующий кластер
            clusters[bestCluster].addTransaction(trans);
        }
        else
        {
            // Создаем новый кластер
            clusters.push_back(newCluster);
            clusters.back().addTransaction(trans);
            bestCluster = static_cast<int>(clusters.size() - 1);
        }

        // Проверяем, изменился ли кластер
        if (bestCluster != currentCluster)
        {
            moves++;
        }
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
        if (!clusters[i].isEmpty() && clusters[i].getCount() > noiseLimit)
        {
            clusterMapping[i] = static_cast<int>(newClusters.size());
            newClusters.push_back(clusters[i]);
        }
    }

    // Обновляем индексы кластеров для транзакций
    for (size_t i = 0; i < ClusTruns.size(); ++i)
    {
        int oldCluster = ClusTruns[i];
        if (oldCluster >= 0 &&
            static_cast<size_t>(oldCluster) < clusterMapping.size() &&
            clusterMapping[oldCluster] != -1)
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
    if (transactions.empty())
        return;

    initClusters(transactions);
    std::cout << "Инициализация завершена. Кластеров: " << clusters.size() << std::endl;

    for (int iter = 0; iter < maxIterations; ++iter)
    {
        int moves = nextStep(transactions);
        std::cout << "Итерация " << iter + 1 << ": перемещений = " << moves
                  << ", кластеров = " << clusters.size() << std::endl;
        if (moves == 0)
        {
            std::cout << "Алгоритм сошелся." << std::endl;
            break;
        }
    }
};

void CLOPE::printClustersInfo() const
{
    std::cout << "\n=== Информация о кластерах ===" << std::endl;
    for (size_t i = 0; i < clusters.size(); ++i)
    {
        std::cout << "Кластер " << i << ": транзакций = " << clusters[i].getCount()
                  << ", площадь = " << clusters[i].getArea()
                  << ", ширина = " << clusters[i].getWidth() << std::endl;
    }
}