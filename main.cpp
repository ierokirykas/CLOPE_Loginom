#include "Clope.h"
#include <iostream>

int main()
{
    // Пример данных: транзакции с целочисленными объектами
    std::vector<std::vector<int>> transactions = {
        {1, 2, 3},
        {1, 2, 4},
        {3, 4, 5},
        {5, 6, 7},
        {1, 2, 3, 4},
        {5, 6}};

    std::cout << "Запуск алгоритма CLOPE..." << std::endl;
    std::cout << "Количество транзакций: " << transactions.size() << std::endl;

    // Создаем и запускаем CLOPE
    CLOPE clope(2.0, 2); // r = 2.0, шумовые кластеры < 2 транзакций
    clope.fit(transactions);

    // Выводим результаты
    std::vector<int> labels = clope.getTransactions();
    std::cout << "\n=== Результаты кластеризации ===" << std::endl;
    for (size_t i = 0; i < labels.size(); ++i)
    {
        std::cout << "Транзакция " << i << " [" << transactions[i].size() << " элементов] -> ";
        if (labels[i] == -1)
            std::cout << "Шум" << std::endl;
        else
            std::cout << "Кластер " << labels[i] << std::endl;
    }

    std::cout << "\nВсего кластеров: " << clope.getNumClusters() << std::endl;

    // Дополнительная информация о кластерах
    clope.printClustersInfo();

    return 0;
}