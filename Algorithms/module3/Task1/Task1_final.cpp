/*
Необходимо написать несколько реализаций интерфейса:
ListGraph, хранящий граф в виде массива списков смежности,
MatrixGraph, хранящий граф в виде матрицы смежности,
SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
ArcGraph, хранящий граф в виде одного массива пар {from, to}.
Также необходимо реализовать конструктор, принимающий const IGraph&.
Такой конструктор должен скопировать переданный граф в создаваемый объект.
Для каждого класса создавайте отдельные h и cpp файлы.
Число вершин графа задается в конструкторе каждой реализации.
*/

#include <iostream>
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"
#include "IGraph.h"

int main()
{
    ListGraph graph(8);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 4);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(2, 5);
    graph.AddEdge(3, 0);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 2);
    graph.AddEdge(4, 6);
    graph.AddEdge(5, 3);
    graph.AddEdge(6, 3);
    graph.AddEdge(6, 5);
    graph.AddEdge(6, 7);
    graph.AddEdge(7, 5);

    graph.mainBFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    graph.mainDFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    std::cout << std::endl;


    MatrixGraph matrix_graph(graph);
    matrix_graph.mainBFS(matrix_graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    matrix_graph.mainDFS(matrix_graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    std::cout << std::endl;


    ArcGraph arc_graph(matrix_graph);
    arc_graph.mainBFS(arc_graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    arc_graph.mainDFS(arc_graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    std::cout << std::endl;

    SetGraph set_graph(arc_graph);
    set_graph.mainBFS(set_graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    set_graph.mainDFS(set_graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

	return 0;
}
