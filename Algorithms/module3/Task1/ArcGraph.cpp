#include "ArcGraph.h"

ArcGraph::ArcGraph(int numVertices) : vertexCount(numVertices){}

ArcGraph::ArcGraph(const IGraph& graph) {
    vertexCount = graph.VerticesCount();
    for (int i = 0; i < vertexCount; ++i) {
        auto nextVertices = graph.GetNextVertices(i);
        for (int nextVertex : nextVertices) {
            adjList.emplace_back(std::pair<int, int>(i, nextVertex));
        }
    }
}

ArcGraph::~ArcGraph(){}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < VerticesCount());
    assert(to >= 0 && to < VerticesCount());
    adjList.emplace_back(std::pair<int, int>(from, to));
}

int ArcGraph::VerticesCount() const {
    return vertexCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < VerticesCount());
    std::vector<int> result;
    for (int i = 0; i < adjList.size(); ++i) {
        if (adjList[i].first == vertex) {
            result.push_back(adjList[i].second);
        }
    }
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < VerticesCount());
    std::vector<int> result;
    for (int i = 0; i < adjList.size(); ++i) {
        if (adjList[i].second == vertex) {
            result.push_back(adjList[i].first);
        }
    }
    return result;
}
