#ifndef _GRAPH_H_
#define _GRAPH_H_
#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

class Graph
{
    int V;    // No. of vertices
    std::list<int> *adj;    // A dynamic array of adjacency lists
public:

    Graph(){

    }

    // Constructor and destructor
    Graph(int V){
        this->V = V;
        adj = new std::list<int>[V];
    }

    ~Graph(){
        delete [] adj;
    }

    // function to read graph as an adjacency list
    void readGraph(std::string path);

    //function to print graph
    void printGraph();

    // function to add an edge to graph
    void addEdge(int v, int w);

    // Prints greedy coloring of the vertices
    void greedyColoring();
};

#endif // _GRAPH_H_
