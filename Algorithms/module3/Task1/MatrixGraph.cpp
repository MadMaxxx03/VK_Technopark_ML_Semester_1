#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int vertexCount) {
    assert(vertexCount > 0);
    adjMatrix.resize(vertexCount);
    for (int i = 0; i < vertexCount; ++i) {
        adjMatrix[i].resize(vertexCount, false);
    }
}

MatrixGraph::MatrixGraph(const IGraph& graph) {
    adjMatrix = std::vector<std::vector<bool>>(graph.VerticesCount(), std::vector<bool>(graph.VerticesCount(), false));
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int elem : nextVertices)
        {
            adjMatrix[i][elem] = true;
        }
    }
}

MatrixGraph::~MatrixGraph(){}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjMatrix.size());
    assert(to >= 0 && to < adjMatrix.size());
    adjMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return adjMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjMatrix.size());
    std::vector<int> nextVertices;
    for (int i = 0; i < adjMatrix.size(); ++i) {
        if (adjMatrix[vertex][i]) {
            nextVertices.push_back(i);
        }
    }
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjMatrix.size());
    std::vector<int> prevVertices;
    for (int i = 0; i < adjMatrix.size(); ++i) {
        if (adjMatrix[i][vertex]) {
            prevVertices.push_back(i);
        }
    }
    return prevVertices;
}
