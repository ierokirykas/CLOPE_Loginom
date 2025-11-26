#include <set>
using namespace std;

class Cluster
{
private:
    multiset<char> Occ{}; // Множество объектов кластера
    int S{0};             // Площадь кластера
    int W{0};             // Ширина кластера
    int N{0};             // Кол-во вхождений (транзакций)

public:
    Cluster(); // Конструктор
    Cluster(int S, int W, int N);
    ~Cluster(); // Деструктор
};