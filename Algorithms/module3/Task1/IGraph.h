#pragma once
#include <vector>
#include <queue>
#include <functional>

struct IGraph {
	virtual ~IGraph() {}

	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

	virtual std::vector<int> GetNextVertices(int vertex) const = 0;
	virtual std::vector<int> GetPrevVertices(int vertex) const = 0;

	void BFS(const IGraph& graph, int vertex, std::vector<bool>& visited, const std::function<void(int)>& func);
	void DFS(const IGraph& graph, int vertex, std::vector<bool>& visited, const std::function<void(int)>& func);

	void mainBFS(const IGraph& graph, const std::function<void(int)>& func);
	void mainDFS(const IGraph& graph, const std::function<void(int)>& func);
};
