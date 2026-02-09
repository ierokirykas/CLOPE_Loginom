#include "Clope.h"
#include "loadMushrooms.h"
#include <iostream>

int main(int argc, char **argv)
{
    // 1. Загрузка данных
    DataLoader loader;
    std::vector<char> labels;

    std::string filename = "mushrooms.txt";
    std::cout << "Загрузка данных из файла: " << filename << std::endl;

    // Загружаем данные, удаляя строки с пропущенными значениями
    std::vector<std::vector<int>> transactions =
        loader.loadMushroomData(filename, labels, true);

    if (transactions.empty())
    {
        std::cerr << "Не удалось загрузить данные или файл пуст." << std::endl;
        return 1;
    }

    std::cout << "Запуск алгоритма CLOPE..." << std::endl;
    std::cout << "Количество транзакций: " << transactions.size() << std::endl;

    // Параметры можно менять для экспериментов:
    double repulsion = atoi(argv[1]); // Параметр отталкивания (чем больше, тем больше кластеров)
    int noiseLimit = 5;               // Минимальный размер кластера (меньше = шум)
    int maxIterations = 20;           // Максимальное количество итераций

    CLOPE clope(repulsion, noiseLimit);
    clope.fit(transactions, maxIterations);

    // Выводим результаты
    std::cout << "\n=== Результаты кластеризации ===" << std::endl;
    for (size_t i = 0; i < labels.size(); ++i)
    {
        if (i % 100 == 0)
        {
            std::cout << "Транзакция " << i << " [" << transactions[i].size() << " элементов] -> ";
            if (labels[i] == -1)
                std::cout << "Шум" << std::endl;
            else
                std::cout << "Кластер " << labels[i] << std::endl;
        }
    }

    std::cout << "\nВсего кластеров: " << clope.getNumClusters() << std::endl;

    // Дополнительная информация о кластерах
    clope.printClustersInfo();

    return 0;
}