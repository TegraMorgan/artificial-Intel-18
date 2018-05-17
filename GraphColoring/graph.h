#ifndef _GRAPH_H_
#define _GRAPH_H_
#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <array>
#include <chrono>
#define MAX_IT 90000
#define log(x) std::cout <<x << std::endl



class Graph
{
    int V;    // No. of vertices
    std::list<int> *adj = nullptr;    // A dynamic array of adjacency lists
    int* colors = nullptr; //colors function
    std::vector<int> usedColors;
    int* conflicts = nullptr;
    unsigned int max_adj;
public:

    unsigned int maxColor;
    long long unsigned int states = 0;
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
        if(adj != nullptr){
            for(int i = 0; i < V;i++){
                adj[i].clear();
            }
            delete[] adj;
            //std::cout << "adj was cleared" << std::endl;
        }
                //log("2");

        if(colors != nullptr){
            delete[] colors;
            //std::cout << "colors was cleared" << std::endl;
        }
                //log("3");


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
    int reduceColors();

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

    int get_minimizing_value(int max_idx);

    void print_conflicts();

    bool minimalConflicts();

    bool minimalConflicts_badEdge();

    bool minimalConflicts_kempeChains();

    void find_conflicts();

    void get_next_pair();
    //find minimum conflicted vertex
    //int find_minimal_conflicted();
    void kempe_chains();
    //find maximum conflicted vertex
    //int find_maximal_conflicted();

    long int objective_func_BE(int* edges, int* bucket, unsigned int segma_C );

    unsigned int objective_func_KC(int* bucket);

    bool isSafe(int v, int color[], int c);


    /* return all the inferences of the assignment */
    bool Inferences(int v, int color[], int c);

    bool isConflicted(int* color);
        /* A recursive utility function to solve m coloring problem */
    bool graphColoringUtil(int m, int color[], int v, int usedColor);

        /* This function solves the m Coloring problem using Backtracking.
      It mainly uses graphColoringUtil() to solve the problem. It returns
      false if the m colors cannot be assigned, otherwise return true and
      prints assignments of colors to all vertices. Please note that there
      may be more than one solutions, this function prints one of the
      feasible solutions.*/
    bool graphColoring(int m);

    bool forwardChecking(int m);

    bool forwardCheckingUtil(int m, int color[], int v,std::vector<int>* domains);

    /* A utility function to print solution */
    void printSolution(int color[]);

};

#endif // _GRAPH_H_
