
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;
class Cluster
{

public:
    unordered_map<int, int> transactions; // Hash-map для транзакций
    int count;                            // Кол-во транкций в кластере
    double area;                          // Площадь гистограммы (сумма всех вхождений)
    double width;                         // Ширина (количество уникальных объектов)

    Cluster() : count(0), area(0.0), width(0.0) {}
    void addTransaction(const vector<int> &transaction);
    void removeTransaction(const vector<int> &transaction);
    double gradient(double r) const;
    double deltaAdd(const vector<int> &transaction, double r) const;
};
