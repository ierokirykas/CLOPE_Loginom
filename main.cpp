#include "Clope.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <random>
#include <map>

using namespace std;

vector<multiset<string>> loadTransactions(const string& filename) {
    vector<multiset<string>> transactions;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return transactions;
    }
    
    string line;
    int lineNum = 0;
    
    while (getline(file, line)) {
        multiset<string> transaction;
        
        // Разбиваем строку по запятым
        vector<string> items;
        stringstream ss(line);
        string item;
        
        while (getline(ss, item, ',')) {
            items.push_back(item);
        }
        
        // Пропускаем строку, если нет хотя бы 2 элементов
        if (items.size() < 2) {
            continue;
        }
        
        // Обрабатываем каждый признак (пропускаем первый столбец - класс)
        for (size_t i = 1; i < items.size(); i++) {
            if (items[i] != "?") {
                // Добавляем признак с индексом для уникальности
                // Формат: значение + "_" + индекс столбца
                string feature = items[i] + "_" + to_string(i-1);
                transaction.insert(feature);
            }
            // Пропускаем '?' как в Python-коде
        }
        
        if (!transaction.empty()) {
            transactions.push_back(transaction);
            lineNum++;
            
            // Для отладки: выводим первые несколько транзакций
            if (lineNum <= 3) {
                cout << "Transaction " << lineNum << " (class: " << items[0] << "): ";
                for (const auto& feature : transaction) {
                    cout << feature << " ";
                }
                cout << endl;
            }
        }
    }
    
    file.close();
    cout << "Successfully loaded " << transactions.size() << " transactions" << endl;
    return transactions;
}
vector<multiset<char>> convertToChar(const vector<multiset<string>>& stringTransactions) {
    vector<multiset<char>> charTransactions;
    
    for (const auto& stringTrans : stringTransactions) {
        multiset<char> charTrans;
        
        // Преобразуем каждую строку в набор символов
        // Просто берем первый символ каждого признака для упрощения
        for (const auto& feature : stringTrans) {
            if (!feature.empty()) {
                charTrans.insert(feature[0]);
            }
        }
        
        charTransactions.push_back(charTrans);
    }
    
    return charTransactions;
}
vector<multiset<char>> loadTransactionsAsChar(const string& filename) {
    vector<multiset<char>> transactions;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return transactions;
    }
    
    string line;
    int lineNum = 0;
    
    while (getline(file, line)) {
        multiset<char> transaction;
        
        vector<string> items;
        stringstream ss(line);
        string item;
        
        while (getline(ss, item, ',')) {
            items.push_back(item);
        }
        
        if (items.size() < 2) {
            continue;
        }
        
        // Обрабатываем каждый признак (пропускаем первый столбец - класс)
        for (size_t i = 1; i < items.size(); i++) {
            if (items[i] != "?" && !items[i].empty()) {
                // Используем первый символ признака + модификатор для уникальности
                // Например: 'x' для x, 'x' + 100 для другого признака
                char baseChar = items[i][0];
                // Добавляем индекс столбца к ASCII коду
                char uniqueChar = baseChar + static_cast<char>(i % 26);
                transaction.insert(uniqueChar);
            }
        }
        
        if (!transaction.empty()) {
            transactions.push_back(transaction);
            lineNum++;
            
            // Для отладки
            if (lineNum <= 3) {
                cout << "Transaction " << lineNum << " (class: " << items[0] << "): ";
                for (char c : transaction) {
                    cout << c << "(" << (int)c << ") ";
                }
                cout << endl;
            }
        }
    }
    
    file.close();
    cout << "Successfully loaded " << transactions.size() << " transactions as char" << endl;
    return transactions;
}

vector<multiset<char>> loadTransactionsWithIndex(const string& filename) {
    vector<multiset<char>> transactions;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return transactions;
    }
    
    // Словарь для уникального кодирования признаков
    map<string, char> featureToChar;
    char nextChar = 'A';  // Начинаем с 'A'
    
    string line;
    int lineNum = 0;
    
    while (getline(file, line)) {
        multiset<char> transaction;
        
        vector<string> items;
        stringstream ss(line);
        string item;
        
        while (getline(ss, item, ',')) {
            items.push_back(item);
        }
        
        if (items.size() < 2) {
            continue;
        }
        
        // Обрабатываем каждый признак
        for (size_t i = 1; i < items.size(); i++) {
            if (items[i] != "?" && !items[i].empty()) {
                // Создаем уникальный ключ: признак + индекс
                string featureKey = items[i] + "_" + to_string(i-1);
                
                // Если признак еще не кодирован, добавляем в словарь
                if (featureToChar.find(featureKey) == featureToChar.end()) {
                    if (nextChar > 'z') {  // Если закончились строчные буквы
                        nextChar = '0';    // Переходим к цифрам
                    } else if (nextChar > '9' && nextChar < 'A') {
                        nextChar = 'A';    // Переходим к заглавным буквам
                    }
                    featureToChar[featureKey] = nextChar;
                    nextChar++;
                }
                
                // Добавляем закодированный признак в транзакцию
                transaction.insert(featureToChar[featureKey]);
            }
        }
        
        if (!transaction.empty()) {
            transactions.push_back(transaction);
            lineNum++;
            
            // Для отладки
            if (lineNum <= 3) {
                cout << "Transaction " << lineNum << " (class: " << items[0] << "): ";
                for (char c : transaction) {
                    // Ищем оригинальный признак для этого символа
                    for (const auto& pair : featureToChar) {
                        if (pair.second == c) {
                            cout << pair.first;
                            break;
                        }
                    }
                }
                cout << endl;
            }
        }
    }
    
    file.close();
    cout << "Successfully loaded " << transactions.size() << " transactions" << endl;
    cout << "Unique features: " << featureToChar.size() << endl;
    
    // Выводим часть таблицы кодирования
    cout << "\nFeature encoding (first 10):" << endl;
    int count = 0;
    for (const auto& pair : featureToChar) {
        if (count++ < 10) {
            cout << pair.first << " -> " << pair.second << endl;
        } else {
            break;
        }
    }
    
    return transactions;
}
int main() {
    cout << "CLOPE Algorithm Implementation - Mushrooms Dataset" << endl;
    cout << "==================================================" << endl;
    
    // Загружаем данные из файла
    string filename = "mushrooms.txt";
    
    // Способ 1: Загрузка с уникальным кодированием признаков
    cout << "\nLoading data with feature encoding..." << endl;
    vector<multiset<char>> transactions = loadTransactionsWithIndex(filename);
    
    if (transactions.empty()) {
        cout << "No transactions loaded! Creating test data..." << endl;
        // Создаем тестовые данные
        vector<string> testStrings = {"ab", "abc", "acd", "de", "def"};
        for (const auto& str : testStrings) {
            multiset<char> transaction;
            for (char c : str) {
                transaction.insert(c);
            }
            transactions.push_back(transaction);
        }
    }
    
    cout << "\nDataset summary:" << endl;
    cout << "Number of transactions: " << transactions.size() << endl;
    
    if (!transactions.empty()) {
        // Статистика по размерам транзакций
        int minSize = INT_MAX, maxSize = 0;
        double avgSize = 0;
        
        for (const auto& trans : transactions) {
            int size = trans.size();
            minSize = min(minSize, size);
            maxSize = max(maxSize, size);
            avgSize += size;
        }
        avgSize /= transactions.size();
        
        cout << "Transaction size - Min: " << minSize 
             << ", Max: " << maxSize 
             << ", Avg: " << avgSize << endl;
    }
    
    // Создание и запуск CLOPE
    double repulsion = 2.6;  // Типичное значение из Python-кода
    cout << "\nRunning CLOPE with repulsion = " << repulsion << endl;
    
    Clope clope(repulsion);
    clope.initialize(transactions);
    clope.startClusterization();
    
    // Результаты
    cout << "\nClustering Results:" << endl;
    cout << "===================" << endl;
    cout << "Number of clusters: " << clope.getClusterCount() << endl;
    
    vector<Cluster> clusters = clope.getClusters();
    
    // Сортируем кластеры по размеру
    sort(clusters.begin(), clusters.end(), 
         [](const Cluster& a, const Cluster& b) {
             return a.getCounter() > b.getCounter();
         });
    
    for (int i = 0; i < clusters.size(); i++) {
        cout << "\nCluster " << i << ":" << endl;
        cout << "  Transactions: " << clusters[i].getCounter() 
             << " (" << (clusters[i].getCounter() * 100.0 / transactions.size()) << "%)" << endl;
        cout << "  Area (S): " << clusters[i].getArea() << endl;
        cout << "  Width (W): " << clusters[i].getWidth() << endl;
        
        // Плотность кластера
        if (clusters[i].getWidth() > 0) {
            double density = clusters[i].getArea() * clusters[i].getCounter() / 
                            pow(clusters[i].getWidth(), repulsion);
            cout << "  Density: " << density << endl;
        }
        
        // Выводим несколько наиболее частых элементов
        cout << "  Top items: ";
        const multiset<char>& occ = clusters[i].getOcc();
        
        // Подсчитываем частоты
        map<char, int> freq;
        for (char c : occ) {
            freq[c]++;
        }
        
        // Сортируем по частоте
        vector<pair<char, int>> freqVec(freq.begin(), freq.end());
        sort(freqVec.begin(), freqVec.end(),
             [](const pair<char, int>& a, const pair<char, int>& b) {
                 return a.second > b.second;
             });
        
        // Выводим топ-5
        int count = 0;
        for (const auto& item : freqVec) {
            if (count++ < 5) {
                cout << item.first << "(" << item.second << ") ";
            } else {
                break;
            }
        }
        cout << endl;
    }
    
    // Анализ качества кластеризации
    cout << "\nQuality Analysis:" << endl;
    cout << "=================" << endl;
    
    double totalDensity = 0;
    for (const auto& cluster : clusters) {
        if (cluster.getWidth() > 0) {
            double density = cluster.getArea() * cluster.getCounter() / 
                            pow(cluster.getWidth(), repulsion);
            totalDensity += density;
        }
    }
    
    cout << "Total density (objective function): " << totalDensity << endl;
    cout << "Average cluster size: " << transactions.size() * 1.0 / clusters.size() << endl;
    
    return 0;
}