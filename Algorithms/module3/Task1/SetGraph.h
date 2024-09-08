#pragma once

#include "IGraph.h"
#include<vector>
#include<cassert>
#include <unordered_set>

class SetGraph : public IGraph {
public:
	SetGraph(int vertices);
	SetGraph(const IGraph& graph);
	~SetGraph();

	// Реализация интерфейса IGraph.
	virtual void AddEdge(int from, int to) override;
	virtual int VerticesCount() const override;
	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::unordered_set<int>> adjSets;
};

