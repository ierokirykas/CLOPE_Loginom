#include "loadMushrooms.h"

std::vector<std::vector<int>> DataLoader::loadMushrooms(const std::string &filename,
                                                        std::vector<char> &labels)
{
    std::vector<std::vector<int>> transactions;
    labels.clear();
    featureToId.clear();
    idToFeature.clear();
    nextId = 1;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Ошибка: не могу открыть файл " << filename << std::endl;
        return transactions;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        // Пропускаем пустые строки
        if (line.empty())
            continue;

        // Разделяем строку по запятым
        std::vector<std::string> tokens = split(line, ',');

        if (tokens.size() != 23)
        {
            std::cerr << "Предупреждение: строка " << lineNumber
                      << " имеет " << tokens.size()
                      << " колонок вместо 23" << std::endl;
            continue;
        }

        // Первый признак - метка съедобности
        char label = tokens[0][0];
        labels.push_back(label);

        // Остальные признаки (22 признака)
        std::vector<int> transaction;

        for (size_t i = 1; i < tokens.size(); i++)
        {
            std::string feature = tokens[i];

            // Пропускаем пропущенные значения (обозначены '?')
            if (feature == "?")
                continue;

            // Добавляем номер признака для уникальности
            std::string uniqueFeature = feature + std::to_string(i);
            int featureId = getFeatureId(uniqueFeature);
            transaction.push_back(featureId);
        }

        // Добавляем транзакцию, даже если в ней есть пропуски
        if (!transaction.empty())
        {
            transactions.push_back(transaction);
        }
    }

    std::cout << "Загружено " << transactions.size() << " транзакций" << std::endl;
    std::cout << "Уникальных признаков: " << featureToId.size() << std::endl;

    return transactions;
}

std::vector<std::vector<int>> DataLoader::loadMushroomData(const std::string &filename,
                                                           std::vector<char> &labels,
                                                           bool removeMissing)
{
    std::vector<std::vector<int>> transactions;
    labels.clear();
    featureToId.clear();
    idToFeature.clear();
    nextId = 1;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Ошибка: не могу открыть файл " << filename << std::endl;
        return transactions;
    }

    std::string line;
    int lineNumber = 0;
    int skippedLines = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        // Пропускаем пустые строки
        if (line.empty())
            continue;

        // Разделяем строку по запятым
        std::vector<std::string> tokens = split(line, ',');

        if (tokens.size() != 23)
        {
            std::cerr << "Предупреждение: строка " << lineNumber
                      << " имеет " << tokens.size()
                      << " колонок вместо 23" << std::endl;
            skippedLines++;
            continue;
        }

        // Первый признак - метка съедобности
        char label = tokens[0][0];

        // Остальные признаки (22 признака)
        std::vector<int> transaction;
        bool hasMissing = false;

        for (size_t i = 1; i < tokens.size(); i++)
        {
            std::string feature = tokens[i];

            // Проверяем пропущенные значения
            if (feature == "?")
            {
                if (removeMissing)
                {
                    hasMissing = true;
                    break; // Пропускаем всю транзакцию
                }
                else
                {
                    continue; // Пропускаем только этот признак
                }
            }

            // Добавляем номер признака для уникальности
            std::string uniqueFeature = feature + std::to_string(i);
            int featureId = getFeatureId(uniqueFeature);
            transaction.push_back(featureId);
        }

        // Если нет пропущенных значений (или мы их игнорируем)
        if (!hasMissing && !transaction.empty())
        {
            transactions.push_back(transaction);
            labels.push_back(label);
        }
        else if (hasMissing)
        {
            skippedLines++;
        }
    }

    std::cout << "Загружено " << transactions.size() << " транзакций" << std::endl;
    std::cout << "Пропущено " << skippedLines << " строк с пропущенными значениями" << std::endl;
    std::cout << "Уникальных признаков: " << featureToId.size() << std::endl;

    // Статистика по меткам
    int edibleCount = 0, poisonousCount = 0;
    for (char label : labels)
    {
        if (label == 'e')
            edibleCount++;
        else if (label == 'p')
            poisonousCount++;
    }
    std::cout << "Съедобных (e): " << edibleCount << std::endl;
    std::cout << "Ядовитых (p): " << poisonousCount << std::endl;

    return transactions;
}

int DataLoader::getFeatureId(const std::string &feature)
{
    auto it = featureToId.find(feature);
    if (it != featureToId.end())
    {
        return it->second;
    }

    int newId = nextId++;
    featureToId[feature] = newId;
    idToFeature[newId] = feature;

    return newId;
}

std::string DataLoader::getFeatureValue(int id) const
{
    auto it = idToFeature.find(id);
    if (it != idToFeature.end())
    {
        return it->second;
    }
    return "UNKNOWN";
}

std::vector<std::string> DataLoader::split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}