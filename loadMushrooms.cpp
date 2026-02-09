#include "loadMushrooms.h"

// Загрузить данные из файла
vector<vector<int>> DataLoader::loadMushrooms(const string &filename,
                                              vector<char> &labels)
{
    vector<vector<int>> transactions;
    labels.clear();
    featureToId.clear();
    idToFeature.clear();
    nextId = 1;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Ошибка: не могу открыть файл " << filename << endl;
        return transactions;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line))
    {
        lineNumber++;

        // Пропускаем пустые строки
        if (line.empty())
            continue;

        // Разделяем строку по запятым
        vector<string> tokens = split(line, ',');

        if (tokens.size() != 23)
        {
            cerr << "Предупреждение: строка " << lineNumber
                 << " имеет " << tokens.size()
                 << " колонок вместо 23" << endl;
            continue;
        }

        // Первый признак - метка съедобности
        char label = tokens[0][0];
        labels.push_back(label);

        // Остальные признаки (22 признака)
        vector<int> transaction;

        for (size_t i = 1; i < tokens.size(); i++)
        {
            string feature = tokens[i];

            // Пропускаем пропущенные значения (обозначены '?')
            if (feature == "?")
                continue;

            // Добавляем номер признака для уникальности
            string uniqueFeature = feature + to_string(i);
            int featureId = getFeatureId(uniqueFeature);
            transaction.push_back(featureId);
        }

        // Добавляем транзакцию, даже если в ней есть пропуски
        if (!transaction.empty())
        {
            transactions.push_back(transaction);
        }
    }

    cout << "Загружено " << transactions.size() << " транзакций" << endl;
    cout << "Уникальных признаков: " << featureToId.size() << endl;

    return transactions;
}

// Загрузить данные для грибов (без первого признака)
vector<vector<int>> DataLoader::loadMushroomData(const string &filename,
                                                 vector<char> &labels,
                                                 bool removeMissing)
{
    vector<vector<int>> transactions;
    labels.clear();
    featureToId.clear();
    idToFeature.clear();
    nextId = 1;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Ошибка: не могу открыть файл " << filename << endl;
        return transactions;
    }

    string line;
    int lineNumber = 0;
    int skippedLines = 0;

    while (getline(file, line))
    {
        lineNumber++;

        // Пропускаем пустые строки
        if (line.empty())
            continue;

        // Разделяем строку по запятым
        vector<string> tokens = split(line, ',');

        if (tokens.size() != 23)
        {
            cerr << "Предупреждение: строка " << lineNumber
                 << " имеет " << tokens.size()
                 << " колонок вместо 23" << endl;
            skippedLines++;
            continue;
        }

        // Первый признак - метка съедобности
        char label = tokens[0][0];

        // Остальные признаки (22 признака)
        vector<int> transaction;
        bool hasMissing = false;

        for (size_t i = 1; i < tokens.size(); i++)
        {
            string feature = tokens[i];

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
            string uniqueFeature = feature + to_string(i);
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

    cout << "Загружено " << transactions.size() << " транзакций" << endl;
    cout << "Пропущено " << skippedLines << " строк с пропущенными значениями" << endl;
    cout << "Уникальных признаков: " << featureToId.size() << endl;

    // Статистика по меткам
    int edibleCount = 0, poisonousCount = 0;
    for (char label : labels)
    {
        if (label == 'e')
            edibleCount++;
        else if (label == 'p')
            poisonousCount++;
    }
    cout << "Съедобных (e): " << edibleCount << endl;
    cout << "Ядовитых (p): " << poisonousCount << endl;

    return transactions;
}

// Преобразовать признак в ID
int DataLoader::getFeatureId(const string &feature)
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

// Получить исходное значение по ID
string DataLoader::getFeatureValue(int id) const
{
    auto it = idToFeature.find(id);
    if (it != idToFeature.end())
    {
        return it->second;
    }
    return "UNKNOWN";
}

// Разделить строку по разделителю
vector<string> DataLoader::split(const string &str, char delimiter)
{
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}