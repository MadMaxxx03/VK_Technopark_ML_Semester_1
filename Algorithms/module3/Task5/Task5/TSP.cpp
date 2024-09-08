#include "TSP.h"
#include "Graph.h"
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <numeric>
#include <tuple>

std::vector<Point> generateRandomPoints(int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dis(0, 1);

    std::vector<Point> points(n);
    for (int i = 0; i < n; ++i) {
        points[i] = { dis(gen), dis(gen) };
    }
    return points;
}

double euclideanDistance(const Point& a, const Point& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double approximateTSP(const std::vector<Point>& points) {
    int n = points.size();
    std::vector<std::tuple<int, int, int>> edges;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int w = static_cast<int>(euclideanDistance(points[i], points[j]) * 1000); 
            edges.push_back(std::make_tuple(w, i, j));
        }
    }

    sortEdges(edges);
    int mst_weight = calculateMSTWeight(n, edges);

    return mst_weight / 1000.0; 
}

void runExperiments(int minN, int maxN, int iterations) {
    for (int n = minN; n <= maxN; ++n) {
        std::vector<double> results;
        for (int i = 0; i < iterations; ++i) {
            auto points = generateRandomPoints(n);
            double mst_weight = approximateTSP(points);
            results.push_back(mst_weight);
        }

        double mean = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
        double sq_sum = std::inner_product(results.begin(), results.end(), results.begin(), 0.0);
        double stddev = std::sqrt(sq_sum / results.size() - mean * mean);

        std::cout << "N: " << n << " Mean: " << mean << " StdDev: " << stddev << std::endl;
    }
}
