#include <vector>
#include <iostream>
#include <unordered_map>
#include <iomanip>
using namespace std;

class Evaluator
{
public:
    static void evaluate(const vector<int> &clusters,
                         const vector<char> &labels);

    static double calculatePurity(const vector<int> &clusters,
                                  const vector<char> &labels);

    static unordered_map<int, unordered_map<char, int>>
    getClusterDistribution(const vector<int> &clusters,
                           const vector<char> &labels);

    static void printClusterStats(const vector<int> &clusters,
                                  const vector<char> &labels);
};