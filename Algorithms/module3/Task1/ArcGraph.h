#include "IGraph.h"
#include <vector>
#include <cassert>

class ArcGraph : public IGraph {
public:
	ArcGraph(int vertexCount);
	ArcGraph(const IGraph& graph);
	~ArcGraph();

	// Реализация интерфейса IGraph.
	virtual void AddEdge(int from, int to) override;
	virtual int VerticesCount() const override;
	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::pair<int, int>> adjList;
	int vertexCount;
};