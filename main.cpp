#include "Clope.h"
#include "loadMushrooms.h"
#include "qualityCheck.h"
#include <iostream>
#include <chrono>

int main(int argc, char **argv)
{

    auto startTime = chrono::high_resolution_clock::now();
    // 1. Загрузка данных
    DataLoader loader;
    vector<char> labels;

    string filename = "mushrooms.txt";

    // Загружаем данные, удаляя строки с пропущенными значениями
    vector<vector<int>> transactions =
        loader.loadMushroomData(filename, labels, true);

    if (transactions.empty())
    {
        cerr << "Не удалось загрузить данные или файл пуст." << endl;
        return 1;
    }

    cout << "Запуск алгоритма CLOPE..." << endl;
    cout << "Количество транзакций: " << transactions.size() << endl;

    // Параметры можно менять для экспериментов:
    double repulsion = atoi(argv[1]); // Параметр отталкивания (чем больше, тем больше кластеров)
    int noiseLimit = 2;               // Минимальный размер кластера (меньше = шум)
    int maxIterations = 10;           // Максимальное количество итераций

    CLOPE clope(repulsion, noiseLimit);
    clope.fit(transactions, maxIterations);

    // Выводим результаты
    cout << "\n=== Результаты кластеризации ===" << endl;
    for (size_t i = 0; i < labels.size(); ++i)
    {
        if (i % 1000 == 0)
        {
            cout << "Транзакция " << i << " [" << transactions[i].size() << " элементов] -> ";
            if (labels[i] == -1)
                cout << "Шум" << endl;
            else
                cout << "Кластер " << labels[i] << endl;
        }
    }

    // 4. Получение результатов
    vector<int> clusterAssignments = clope.getTransactions();
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration<double>(endTime - startTime);
    cout << "\n=== РЕЗУЛЬТАТЫ ===" << endl;
    cout << "Время выполнения: " << fixed << setprecision(3)
         << duration.count() << " секунд" << endl;
    cout << "Кластеров получено: " << clope.getNumClusters() << endl;

    // 5. Оценка качества
    Evaluator::evaluate(clusterAssignments, labels);

    return 0;
}