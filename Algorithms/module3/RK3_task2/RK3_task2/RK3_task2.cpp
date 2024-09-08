//Нужно проверить, является ли путь в неориентированном графе гамильтоновым.

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <set>

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
		prevAdjLists.resize(vertexCount);
	}

	ListGraph(const IGraph& graph) {
		adjLists.resize(graph.VerticesCount());
		prevAdjLists.resize(graph.VerticesCount());
		for (int i = 0; i < VerticesCount(); ++i) {
			adjLists[i] = graph.GetNextVertices(i);
			prevAdjLists[i] = graph.GetPrevVertices(i);
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
		return prevAdjLists[vertex];
	}

private:
	std::vector<std::vector<int>> adjLists;
	std::vector<std::vector<int>> prevAdjLists;
};

bool isHamiltonianPath(const IGraph& graph, const std::vector<int>& path) {
	if (path.size() != graph.VerticesCount()) {
		return false;
	}

	std::set<int> visited;
	for (int i = 0; i < path.size() - 1; ++i) {
		int current = path[i];
		int next = path[i + 1];

		const auto& neighbors = graph.GetNextVertices(current);
		if (std::find(neighbors.begin(), neighbors.end(), next) == neighbors.end()) {
			return false;
		}
		visited.insert(current);
	}
	visited.insert(path.back());

	return visited.size() == graph.VerticesCount();
}

int main() {
	int N = 0, M = 0, K = 0;
	std::cin >> N >> M >> K;
	ListGraph graph(N);

	for (int i = 0; i < M; ++i) {
		int from = 0, to = 0;
		std::cin >> from >> to;
		graph.AddEdge(from, to);
	}

	std::vector<int> path(K);
	for (int i = 0; i < K; ++i) {
		std::cin >> path[i];
	}

	std::cout << (isHamiltonianPath(graph, path) ? 1 : 0) << std::endl;

	return 0;
}
