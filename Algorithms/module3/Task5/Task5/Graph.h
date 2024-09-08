#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <tuple>

std::vector<std::tuple<int, int, int>> readEdges(int m);
void sortEdges(std::vector<std::tuple<int, int, int>>& edges);
int calculateMSTWeight(int n, const std::vector<std::tuple<int, int, int>>& edges);

#endif // GRAPH_H
