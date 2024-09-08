#include "SetGraph.h"

SetGraph::SetGraph(int vertices) : adjSets(vertices) {}

SetGraph::SetGraph(const IGraph& graph) : adjSets(graph.VerticesCount()) {
    for (int vertex = 0; vertex < graph.VerticesCount(); ++vertex) {
        const std::vector<int>& nextVertices = graph.GetNextVertices(vertex);
        adjSets[vertex].insert(nextVertices.begin(), nextVertices.end());
    }
}

SetGraph::~SetGraph(){}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjSets.size());
    assert(to >= 0 && to < adjSets.size());
    adjSets[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return adjSets.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjSets.size());
    return std::vector<int>(adjSets[vertex].begin(), adjSets[vertex].end());
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjSets.size());
    std::unordered_set<int> prevVertices;
    for (int i = 0; i < adjSets.size(); ++i) {
        if (adjSets[i].count(vertex)) {
            prevVertices.insert(i);
        }
    }
    return std::vector<int>(prevVertices.begin(), prevVertices.end());
}
