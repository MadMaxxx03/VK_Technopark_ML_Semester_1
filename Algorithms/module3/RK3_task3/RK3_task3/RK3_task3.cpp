/*
Рик и Морти снова бороздят просторы вселенных,
но решили ограничиться только теми, номера которых меньше M.
Они могут телепортироваться из вселенной с номером z во
вселенную (z+1) mod M за a бутылок лимонада или во вселенную
(z2+1) mod M за b бутылок лимонада. Рик и Морти хотят добраться
из вселенной с номером x во вселенную с номером y.
Сколько бутылок лимонада отдаст Рик за такое путешествие,
если он хочет потратить их как можно меньше?
*/

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

struct Edge {
    int to;
    long long weight;
};

class Graph {
public:
    Graph(int vertexCount) : adjLists(vertexCount) {}

    void AddEdge(int from, int to, long long weight) {
        if (from >= 0 && from < adjLists.size() && to >= 0 && to < adjLists.size()) {
            adjLists[from].push_back({ to, weight });
        }
        else {
            std::cerr << "AddEdge: index out of range (from: " << from << ", to: " << to << ")" << std::endl;
        }
    }

    const std::vector<Edge>& GetNextVertices(int vertex) const {
        if (vertex >= 0 && vertex < adjLists.size()) {
            return adjLists[vertex];
        }
        else {
            std::cerr << "GetNextVertices: index out of range (vertex: " << vertex << ")" << std::endl;
            static std::vector<Edge> empty;
            return empty;
        }
    }

    int VerticesCount() const {
        return adjLists.size();
    }

private:
    std::vector<std::vector<Edge>> adjLists;
};

long long FindMinLemonade(long long a, long long b, int M, int x, int y) {
    Graph graph(M);

    for (int i = 0; i < M; ++i) {
        graph.AddEdge(i, (int)((i + 1) % M), a);
        graph.AddEdge(i, (int)(((long long)i * i + 1) % M), b);
    }

    std::vector<long long> minBottles(M, LLONG_MAX);
    minBottles[x] = 0;
    std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, std::greater<>> pq;
    pq.push({ 0, x });

    while (!pq.empty()) {
        long long currentBottles = pq.top().first;
        int currentVertex = pq.top().second;
        pq.pop();

        if (currentBottles > minBottles[currentVertex]) {
            continue;
        }

        for (const Edge& edge : graph.GetNextVertices(currentVertex)) {
            int nextVertex = edge.to;
            long long newBottles = currentBottles + edge.weight;
            if (nextVertex >= 0 && nextVertex < M && newBottles < minBottles[nextVertex]) {
                minBottles[nextVertex] = newBottles;
                pq.push({ newBottles, nextVertex });
            }
            else if (nextVertex < 0 || nextVertex >= M) {
                std::cerr << "FindMinLemonade: nextVertex out of range (nextVertex: " << nextVertex << ")" << std::endl;
            }
        }
    }

    return minBottles[y] == LLONG_MAX ? -1 : minBottles[y];
}

int main() {
    long long a = 0, b = 0;
    int M = 0, x = 0, y = 0;
    std::cin >> a >> b >> M >> x >> y;
    std::cout << FindMinLemonade(a, b, M, x, y) << std::endl;
    return 0;
}
