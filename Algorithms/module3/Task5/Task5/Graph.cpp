#include "Graph.h"
#include "DSU.h"
#include <algorithm>
#include <tuple>
#include <vector>
#include <iostream>

std::vector<std::tuple<int, int, int>> readEdges(int m) {
    std::vector<std::tuple<int, int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        int b, e, w;
        std::cin >> b >> e >> w;
        edges[i] = std::make_tuple(w, b - 1, e - 1); // Индексация с 0
    }
    return edges;
}

void sortEdges(std::vector<std::tuple<int, int, int>>& edges) {
    std::sort(edges.begin(), edges.end());
}

int calculateMSTWeight(int n, const std::vector<std::tuple<int, int, int>>& edges) {
    DSU dsu(n);
    int mst_weight = 0;

    for (const auto& edge : edges) {
        int w, b, e;
        std::tie(w, b, e) = edge;
        if (dsu.FindParent(b) != dsu.FindParent(e)) {
            dsu.Union(b, e);
            mst_weight += w;
        }
    }

    return mst_weight;
}
