//Посчитать кол-во компонент связности в неориентированном графе.

#include <iostream>
#include <vector>
#include <cassert>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
public:
    ListGraph(int vertexCount) {
        assert(vertexCount > 0);
        adjLists.resize(vertexCount);
    }

    ListGraph(const IGraph& graph) {
        adjLists.resize(graph.VerticesCount());
        for (int i = 0; i < VerticesCount(); ++i) {
            adjLists[i] = graph.GetNextVertices(i);
        }
    }

    virtual void AddEdge(int from, int to) override {
        assert(from >= 0 && from < adjLists.size());
        assert(to >= 0 && to < adjLists.size());
        adjLists[from].push_back(to);
        adjLists[to].push_back(from); 
    }

    virtual int VerticesCount() const override {
        return adjLists.size();
    }

    virtual std::vector<int> GetNextVertices(int vertex) const override {
        assert(vertex >= 0 && vertex < adjLists.size());
        return adjLists[vertex];
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const override {
        assert(vertex >= 0 && vertex < adjLists.size());
        return adjLists[vertex]; 
    }

private:
    std::vector<std::vector<int>> adjLists;
};

void DFS(const ListGraph& graph, int vertex, std::vector<bool>& visited) {
    visited[vertex] = true;
    for (int next : graph.GetNextVertices(vertex)) {
        if (!visited[next]) {
            DFS(graph, next, visited);
        }
    }
}

int CountConnectedComponents(const ListGraph& graph) {
    int components = 0;
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        if (!visited[i]) {
            DFS(graph, i, visited);
            ++components;
        }
    }
    return components;
}

int main() {
    int N = 0, M = 0;
    std::cin >> N >> M;
    ListGraph graph(N);
    for (int i = 0; i < M; ++i) {
        int from = 0, to = 0;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::cout << CountConnectedComponents(graph) << std::endl;

    return 0;
}
