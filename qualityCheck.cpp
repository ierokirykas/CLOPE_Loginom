#include "qualityCheck.h"
#include <algorithm>
#include <cmath>

// Оценка качества кластеризации
void Evaluator::evaluate(const vector<int> &clusters,
                         const vector<char> &labels)
{
    if (clusters.size() != labels.size())
    {
        cerr << "Ошибка: размеры кластеров и меток не совпадают" << endl;
        return;
    }

    cout << "\n=== ОЦЕНКА КАЧЕСТВА КЛАСТЕРИЗАЦИИ ===\n"
         << endl;

    // 1. Подсчет количества кластеров
    int maxCluster = *max_element(clusters.begin(), clusters.end());
    int minCluster = *min_element(clusters.begin(), clusters.end());

    // Подсчет уникальных кластеров (исключая шум -1)
    unordered_map<int, int> clusterCounts;
    int noiseCount = 0;

    for (int cluster : clusters)
    {
        if (cluster == -1)
        {
            noiseCount++;
        }
        else
        {
            clusterCounts[cluster]++;
        }
    }

    cout << "Всего кластеров: " << clusterCounts.size() << endl;
    cout << "Транзакций, помеченных как шум: " << noiseCount
         << " (" << fixed << setprecision(2)
         << (noiseCount * 100.0 / clusters.size()) << "%)" << endl;

    // 2. Чистота кластеризации
    double purity = calculatePurity(clusters, labels);
    cout << "Чистота кластеризации: " << fixed << setprecision(4)
         << purity << endl;

    // 3. Распределение по кластерам
    printClusterStats(clusters, labels);
}

// Оценить чистоту кластеров
double Evaluator::calculatePurity(const vector<int> &clusters,
                                  const vector<char> &labels)
{
    auto distribution = getClusterDistribution(clusters, labels);

    int totalCorrect = 0;
    int totalItems = 0;

    for (const auto &clusterPair : distribution)
    {
        int maxCount = 0;
        int clusterTotal = 0;

        for (const auto &labelPair : clusterPair.second)
        {
            clusterTotal += labelPair.second;
            if (labelPair.second > maxCount)
            {
                maxCount = labelPair.second;
            }
        }

        totalCorrect += maxCount;
        totalItems += clusterTotal;
    }

    if (totalItems == 0)
        return 0.0;
    return static_cast<double>(totalCorrect) / totalItems;
}

// Посчитать распределение меток по кластерам
unordered_map<int, unordered_map<char, int>>
Evaluator::getClusterDistribution(const vector<int> &clusters,
                                  const vector<char> &labels)
{
    unordered_map<int, unordered_map<char, int>> distribution;

    for (size_t i = 0; i < clusters.size(); ++i)
    {
        int cluster = clusters[i];
        if (cluster != -1)
        { // Игнорируем шум
            char label = labels[i];
            distribution[cluster][label]++;
        }
    }

    return distribution;
}

// Вывести статистику по кластерам
void Evaluator::printClusterStats(const vector<int> &clusters,
                                  const vector<char> &labels)
{
    auto distribution = getClusterDistribution(clusters, labels);

    cout << "\nРаспределение по кластерам:\n"
         << endl;
    cout << left << setw(15) << "Кластер"
         << setw(15) << "Всего"
         << setw(15) << "e(съед.)"
         << setw(12) << "p(яд.)"
         << setw(25) << "Преобладает"
         << setw(15) << "Чистота"
         << endl;
    cout << string(70, '-') << endl;

    for (const auto &clusterPair : distribution)
    {
        int clusterId = clusterPair.first;
        const auto &labelCounts = clusterPair.second;

        int total = 0;
        int edibleCount = 0;
        int poisonousCount = 0;

        for (const auto &labelPair : labelCounts)
        {
            total += labelPair.second;
            if (labelPair.first == 'e')
            {
                edibleCount = labelPair.second;
            }
            else if (labelPair.first == 'p')
            {
                poisonousCount = labelPair.second;
            }
        }

        char dominant = (edibleCount > poisonousCount) ? 'e' : 'p';
        int dominantCount = (edibleCount > poisonousCount) ? edibleCount : poisonousCount;
        double clusterPurity = static_cast<double>(dominantCount) / total;

        cout << left << setw(10) << clusterId
             << setw(10) << total
             << setw(10) << edibleCount
             << setw(10) << poisonousCount
             << setw(21) << (dominant == 'e' ? "съедобный" : "ядовитый")
             << setw(15) << fixed << setprecision(3) << clusterPurity
             << endl;
    }
}