#include "IGraph.h"

void IGraph::BFS(const IGraph& graph, int vertex, 
    std::vector<bool>& visited, const std::function<void(int)>& func){ 
    std::queue<int> q;
    q.push(vertex);
    visited[vertex] = true;

    while (!q.empty()){
        int currentVertex = q.front();
        q.pop();

        func(currentVertex);

        for (int nextVertex : graph.GetNextVertices(currentVertex)){
            if (!visited[nextVertex]){
                visited[nextVertex] = true;
                q.push(nextVertex);
            }
        }
    }
}

void IGraph::DFS(const IGraph& graph, int vertex,
    std::vector<bool>& visited, const std::function<void(int)>& func) {
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex : graph.GetNextVertices(vertex)) {
        if (!visited[nextVertex]) {
            DFS(graph, nextVertex, visited, func);
        }
    }
}

void IGraph::mainBFS(const IGraph& graph, const std::function<void(int)>& func){
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i){
        if (!visited[i]){
            BFS(graph, i, visited, func);
        }
    }
}

void IGraph::mainDFS(const IGraph& graph, const std::function<void(int)>& func){
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i){
        if (!visited[i]){
            DFS(graph, i, visited, func);
        }
    }
}