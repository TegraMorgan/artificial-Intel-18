#ifndef _GRAPH_H_
#define _GRAPH_H_
#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <algorithm>

#define MAX_IT 500000
class Graph
{
    int V;    // No. of vertices
    std::list<int> *adj;    // A dynamic array of adjacency lists
    int* colors; //colors function
    std::vector<int> usedColors;
    int* conflicts;
public:

    int maxColor;

    Graph(){

    }

    // Constructor and destructor
    Graph(int V){
        this->V = V;
        adj = new std::list<int>[V];
    }

    Graph(Graph& other){
        this->V = other.getV();
        this->adj = new std::list<int>[V];
        this-> adj = other.getAdjacency();
//        std::list<int>* o = other.getAdjacency();
//        for (int u = 0; u < V; u++){
////            std::list<int>::iterator j;
////            for (j = o[u].begin(); j != o[u].end();++j){
////                    std::cout << *j << " ###" << std::endl;
////                    adj[u].push_back(*j);
////            }
//           // adj[u] = std::list<int>(o[u]);
//           for (auto const& i : o[u]) {
//                this->adj[u].push_back(i);
//            }
//
//        }
        //other.printit();
        //this->printit();
        this->colors = new int[V];
        int* other_colors = other.getColors();
        for(int i = 0; i < V; i++){
            this->colors[i] = other_colors[i];
        }
        this->usedColors = std::vector<int>(other.getUsedColors());
    }

    ~Graph(){
        if(adj){
            for(int i = 0; i < V;i++){
                adj[i].clear();
            }
            delete[] adj;
            std::cout << "adj was cleared" << std::endl;
        }

        if(colors){
            delete[] colors;
            std::cout << "colors was cleared" << std::endl;
        }
        this->usedColors.clear();

    }

    //get adjacency
    std::list<int>* getAdjacency();

    //return current colors
    int* getColors();

    //return number of vertices
    int getV();

    // function to read graph as an adjacency list
    void readGraph(std::string path);

    //function to print graph
    void printGraph();

    // function to add an edge to graph
    void addEdge(int v, int w);

    // Prints greedy coloring of the vertices
    void greedyColoring();

    //decrease colors used by 1
    void reduceColors();

    //set colors
    void setColors(int* new_colors){
        delete[] this->colors;
        this->colors = new_colors;
    }

    std::vector<int> getUsedColors();

    void printit(){
        std::list<int>* o = this->getAdjacency();
        for (int u = 0; u < V; u++){
            std::list<int>::iterator j;
            for (j = o[u].begin(); j != o[u].end();++j){
                    std::cout << *j << " ###" << std::endl;

            }


        }
    }

    bool minimalConflicts();

    //find minimum conflicted vertex
    int find_minimal_conflicted();

    //find maximum conflicted vertex
    int find_maximal_conflicted();

};

#endif // _GRAPH_H_
