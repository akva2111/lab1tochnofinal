#include <vector>      
#include <iostream>    
#include <limits> 
#include <random>
#include <chrono> 
#include <iomanip>
#include <sstream>
using CostMatrix = std::vector<std::vector<int>>;
using Path = std::vector<int>;//1234
const int INF = std::numeric_limits<int>::max();
std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
// --- best rezultat ---
struct TspSolution {
    Path bestPath;
    int minCost = INF;
};
void fillRandomMatrix(CostMatrix& matrix, int numCities, int maxCost, int startCity = 0) {

    matrix.assign(numCities, std::vector<int>(numCities));
    // gen rand num
    std::random_device rd; // inicializaci gen
    std::mt19937 gen(rd()); // gen num ( Mersenne Twister)
    std::uniform_int_distribution<> distrib(1, maxCost); // from 1 to maxCost

    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (i == j) {
                matrix[i][j] = 0; // price diagonal 0
            }
            else {
                matrix[i][j] = distrib(gen); // rand price
            }
        }
    }
}
//funkt find full prica road
int calculatePathCost(const CostMatrix& matrix, const Path& path) {
    if (path.size() < 2) {
        return 0;
    }
    if (matrix.empty()) {
        std::cerr << "[calculatePathCost] Eror: matrix void." << std::endl;
        return INF;
    }
    int totalCost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int fromCity = path[i];
        int toCity = path[i + 1];

        // find bug in index
        if (fromCity < 0 || fromCity >= matrix.size()
            || toCity < 0 || toCity >= matrix.size()) {
            return INF;
        }
        int segmentCost = matrix[fromCity][toCity];

        if (segmentCost == INF) {
            return INF;
        }
        // find over level
        if (totalCost > INF - segmentCost) {
            return INF;
        }
        totalCost += segmentCost;
    }
    return totalCost;
}
bool nextPermutationDijkstra(std::vector<int>& p) {
    int n = p.size();
    // 1. find max i for, 0 < i < n è P[i] < P[i+1].
    int i = -1;
    for (int k = n - 2; k >= 0; --k) {
        if (p[k] < p[k + 1]) {
            i = k;
            break;
        }
    }

    if (i == -1) {
        return false; // next permut
    }

    // 2. find max j for ,i < j <= n and P[i] < P[j].
    int j = -1;
    for (int k = n - 1; k > i; --k) {
        if (p[i] < p[k]) {
            j = k;
            break;
        }
    }

    // 3. swap P[i] and P[j].
    std::swap(p[i], p[j]);
    //4
    int left = i + 1;
    int right = n - 1;
    while (left < right) {
        std::swap(p[left], p[right]);
        left++;
        right--;
    }

    return true;
}
//Tsp
TspSolution solveTspBruteForce(const CostMatrix& matrix, int startCity) {
    int numCities = matrix.size();
    if (numCities <= 0) {
        std::cerr << "[solveTspBruteForce74] Eror:matrix void (numCities <= 0)." << std::endl;
        return { {}, 0 }; // void matrix
        if (startCity < 0 || startCity >= numCities) {
            std::cerr << "[solveTspBruteForce77] Eror start City: " << startCity << ", ??? " << numCities << " ???????." << std::endl;
            return { {}, INF };
        }
    }
    if (numCities == 1) {
        return { {startCity, startCity}, 0 }; // one town noproblem
    }

    TspSolution solution;
    solution.minCost = INF;
    std::vector<int> citiesToPermute;
    citiesToPermute.reserve(numCities - 1); // save member
    for (int i = 0; i < numCities; ++i) {
        if (i != startCity) {
            citiesToPermute.push_back(i);
        }
    }
    do {
        //  startCity -> permutation -> startCity
        Path currentPath;
        currentPath.reserve(numCities + 1); // start
        currentPath.push_back(startCity); // start from start
        for (int city : citiesToPermute) {
            currentPath.push_back(city);
        }
        currentPath.push_back(startCity); // end in stars

        // find all price
        int currentCost = calculatePathCost(matrix, currentPath);

        // fiend best
        if (currentCost < solution.minCost) {
            solution.minCost = currentCost;
            solution.bestPath = currentPath;
        }
    } while (nextPermutationDijkstra(citiesToPermute));
    if (solution.minCost == INF) {
        std::cerr << "[solveTspBruteForce115] danger!! all path INF." << std::endl;
    }
    return solution;
}

void main() {

    int numCities, startCity, maxCost;
    // how much City
    std::cout << "input how much City: ";
    std::cin >> numCities;
    while (numCities <= 0) {
        std::cerr << "Eror: City must be>0, try again: ";
        std::cin >> numCities;
    }

    // start sity
    std::cout << "input start town (from 0 to " << numCities - 1 << "): ";
    std::cin >> startCity;
    while (startCity < 0 || startCity >= numCities) {
        std::cerr << "Eror: not corect number start Town: ";
        std::cin >> startCity;
    }
    // input Max price
    std::cout << "input Max price: ";
    std::cin >> maxCost;
    while (maxCost < 0) {
        std::cerr << "Eror: Max price must be>0. try again: ";
        std::cin >> maxCost;
    }

    // --- start ---
    CostMatrix demoMatrix;
    fillRandomMatrix(demoMatrix, numCities, maxCost, startCity);

    std::cout << "\n--- solve ---" << std::endl;
    std::cout << "Number of cities: " << numCities << std::endl;
    std::cout << "start citi: " << startCity << std::endl;
    std::cout << "max price: " << maxCost << std::endl;

    std::cout << "\nMatrix price:" << std::endl;
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (demoMatrix[i][j] == INF) std::cout << " INF ";
            else std::cout << std::setw(4) << demoMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    auto demoStartTime = std::chrono::high_resolution_clock::now();
    TspSolution demoSolution = solveTspBruteForce(demoMatrix, startCity);
    auto demoEndTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> demoElapsed = demoEndTime - demoStartTime;

    std::cout << "\nresult:" << std::endl;
    std::cout << "  min price: " << (demoSolution.minCost == INF ? "INF" : intToString(demoSolution.minCost)) << std::endl;
    std::cout << "  Best Path: ";
    for (size_t i = 0; i < demoSolution.bestPath.size(); ++i) {
        std::cout << demoSolution.bestPath[i] << (i == demoSolution.bestPath.size() - 1 ? "" : " -> ");
    }
    std::cout << std::endl;
    std::cout << "  time: " << demoElapsed.count() << " sec." << std::endl;
}