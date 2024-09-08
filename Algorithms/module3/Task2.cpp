/*
Дан невзвешенный неориентированный граф. 
В графе может быть несколько кратчайших путей между какими-то вершинами. 
Найдите количество различных кратчайших путей между заданными вершинами.
*/

#include <sstream>
#include <iostream>
#include<vector>
#include<cassert>
#include <queue>

struct IGraph {
	virtual ~IGraph() {}

	// Добавление ребра от from к to.
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

	// IGraph
	virtual void AddEdge(int from, int to) override {
		assert(from >= 0 && from < adjLists.size());
		assert(to >= 0 && to < adjLists.size());
		adjLists[from].push_back(to);
		prevAdjLists[to].push_back(from);
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

int GetShortestPathsCount(const IGraph& g, int from, int to) {
	int vertices = g.VerticesCount();
	std::vector<bool> visited(vertices, false);
	std::vector<size_t>pathsCount(vertices, 0);
	std::deque<int> toVisit;

	pathsCount[from] = 1;
	visited[from] = true;
	toVisit.emplace_back(from);

	while (true) {
		std::vector<bool> visitedNow(vertices, false);
		std::deque<int> adjacent;

		while (!toVisit.empty()) {
			int currentVertex = toVisit.front();
			toVisit.pop_front();

			for (const int& nextVertex : g.GetNextVertices(currentVertex)) {
				if (!visited[nextVertex]) {
					pathsCount[nextVertex] += pathsCount[currentVertex];
					if (!visitedNow[nextVertex])
						adjacent.emplace_back(nextVertex);
					visitedNow[nextVertex] = true;
				}
			}

		}
		if (adjacent.empty()) 
			return pathsCount[to];

		if (visitedNow[to]) 
			return pathsCount[to];

		for (const int& nextVertex : adjacent)
			visited[nextVertex] = true;

		toVisit = adjacent;
	}
}


int main() {
	int v = 0, n = 0;
	std::cin >> v >> n;
	int from = 0, to = 0;
	ListGraph graph(v);
	for (int i = 0; i < n; ++i) {
		std::cin >> from >> to;
		graph.AddEdge(from, to);
		graph.AddEdge(to, from);
	}
	int u = 0, w = 0;
	std::cin >> u >> w;
	std::cout << GetShortestPathsCount(graph, u, w);

	return 0;
}
