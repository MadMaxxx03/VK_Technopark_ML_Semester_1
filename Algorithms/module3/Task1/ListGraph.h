#pragma once

#include "IGraph.h"
#include<vector>
#include<cassert>

class ListGraph : public IGraph {
public:
	ListGraph(int vertexCount);
	ListGraph(const IGraph& graph);

	// Реализация интерфейса IGraph.
	virtual void AddEdge(int from, int to) override;
	virtual int VerticesCount() const override;
	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::vector<int>> adjLists;
	std::vector<std::vector<int>> prevAdjLists;
};

