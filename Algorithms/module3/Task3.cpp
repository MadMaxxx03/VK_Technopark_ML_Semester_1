/*
Требуется отыскать самый выгодный маршрут между городами.
Требования: время работы O((N+M)logN), 
где N-количество городов, M-известных дорог между ними.

Формат входных данных.
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).
Формат выходных данных.
Вывести длину самого выгодного маршрута.
*/

#include <climits>
#include <iostream>
#include <vector>
#include <queue>
#include <cassert>

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to, size_t weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual int ShortestPath(const int from, const int to) const = 0;

    virtual std::vector<std::pair<int, size_t>> GetNextVertices(int vertex) const = 0;
    virtual std::vector<std::pair<int, size_t>> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
public:
    ListGraph() = default;

    ListGraph(const size_t vertices) :
        vertices(vertices), adjLists(vertices) {}

    ListGraph(const IGraph& graph) :
        vertices(graph.VerticesCount()),
        adjLists(vertices)
    {
        for (int i = 0; i < vertices; ++i) {
            for (const auto& vertex : graph.GetNextVertices(i))
                adjLists[i].emplace_back(vertex);
        }
    }

    ~ListGraph() = default;

    virtual void AddEdge(int from, int to, size_t weight) override {
        assert(from >= 0 && from < adjLists.size());
        assert(to >= 0 && to < adjLists.size());
        adjLists[from].emplace_back(to, weight);
    }

    virtual int VerticesCount() const override {
        return vertices;
    }

    virtual int ShortestPath(int from, int to) const override {
        assert(from >= 0 && from < adjLists.size());
        assert(to >= 0 && to < adjLists.size());

        std::vector<size_t> distances(vertices, LONG_MAX);
        distances[from] = 0;
        using PII = std::pair<size_t, int>;
        std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;
        pq.emplace(0, from);

        while (!pq.empty()) {
            int curr_v = pq.top().second;
            size_t curr_dist = pq.top().first;
            pq.pop();

            if (curr_dist > distances[curr_v]) continue;

            for (const auto& next : GetNextVertices(curr_v)) {
                int next_v = next.first;
                size_t weight = next.second;

                if (distances[curr_v] + weight < distances[next_v]) {
                    distances[next_v] = distances[curr_v] + weight;
                    pq.emplace(distances[next_v], next_v);
                }
            }
        }

        return distances[to];
    }

    virtual std::vector<std::pair<int, size_t>> GetNextVertices(int vertex) const override {
        assert(vertex >= 0 && vertex < adjLists.size());
        return adjLists[vertex];
    }

    virtual std::vector<std::pair<int, size_t>> GetPrevVertices(int vertex) const override {
        assert(vertex >= 0 && vertex < adjLists.size());
        std::vector<std::pair<int, size_t>> prev;

        for (size_t vertex = 0; vertex < adjLists.size(); ++vertex) {
            for (const auto& adjacent : adjLists[vertex])
                if (adjacent.first == vertex)
                    prev.emplace_back(vertex, adjacent.second);
        }

        return prev;
    }

private:
    size_t vertices = 0;
    std::vector<std::vector<std::pair<int, size_t>>> adjLists;
};

int main() {
    size_t vertices = 0, edges = 0;
    std::cin >> vertices;
    std::cin >> edges;
    ListGraph graph(vertices);

    for (size_t i = 0; i < edges; ++i) {
        int from = 0, to = 0;
        size_t weight = 0;
        std::cin >> from >> to >> weight;

        graph.AddEdge(from, to, weight);
        graph.AddEdge(to, from, weight);
    }

    int from = 0, to = 0;
    std::cin >> from >> to;
    std::cout << graph.ShortestPath(from, to) << std::endl;

    return 0;
}