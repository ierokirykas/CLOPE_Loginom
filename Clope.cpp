#include "Clope.h"
#include <cmath>
Clope::Clope() = default;
Clope::~Clope() = default;

double Clope::deltaAdd(const Cluster &cluster, const multiset<char> &transaction, double r)
{
    int area_new = cluster.getArea() + transaction.size();
    int width_new = cluster.getWidth();

    for (char item : transaction)
    {
        if (!cluster.containsItem(item))
        {
            width_new++;
        }
    }

    if (width_new == 0)
    {
        return 0.0;
    }

    double new_value = area_new * (cluster.getCounter() + 1) / pow(width_new, r);
    double old_value = 0.0;

    if (cluster.getWidth() > 0)
    {
        cluster.getArea() * cluster.getCounter() / pow(cluster.getWidth(), r);
    }
    else
        return 0.0;

    return new_value - old_value;
}
