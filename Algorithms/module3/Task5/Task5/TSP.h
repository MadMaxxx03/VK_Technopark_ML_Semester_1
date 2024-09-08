#ifndef TSP_H
#define TSP_H

#include <vector>
#include <cmath>

struct Point {
    double x, y;
};

std::vector<Point> generateRandomPoints(int n);
double euclideanDistance(const Point& a, const Point& b);
double approximateTSP(const std::vector<Point>& points);
void runExperiments(int minN, int maxN, int iterations);

#endif // TSP_H
