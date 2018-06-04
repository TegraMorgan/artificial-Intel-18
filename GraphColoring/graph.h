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
    std::list<int> *adj = nullptr;    // adjacency lists
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
        }


        if(colors != nullptr){
            delete[] colors;
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
    int reduceColors();

    //set colors
    void setColors(int* new_colors){
        delete[] this->colors;
        this->colors = new_colors;
    }

    //get vector of used colors
    std::vector<int> getUsedColors();


    int get_minimizing_value(int max_idx);

    void print_conflicts();
    //Local search - feasibility
    bool minimalConflicts();

    //Local search - hybridization
    bool minimalConflicts_badEdge();

    //Local search - Kempe
    bool minimalConflicts_kempeChains();

    //function that finds the conflicts in the graph
    void find_conflicts();

    //this function used upon finding the kempe chains
    void get_next_pair();

    //finds next kempe chain for colors returned by get_next_pair()
    void kempe_chains();

    //objective function for bad edge
    long int objective_func_BE(int* edges, int* bucket, unsigned int segma_C );

    //objective function for kempe-chains
    unsigned int objective_func_KC(int* bucket);

    //checks if color is valid for vertex v
    bool isSafe(int v, int color[], int c);

    //returns true if graph has invalid coloring, false otherwise
    bool isConflicted(int* color);

    // A recursive utility function to solve m coloring problem using backJumping
    bool backJumpingColoringUtil(int m, int color[], int v, int usedColor);


    //This function solves the coloring problem using BackJumping.
    //It mainly uses backJumpingColoringUtil() to solve the problem. It returns
    //false if the graph cannot be colored with m colors, otherwise return true
    bool backJumpingGraphColoring(int m);

    //This is the forward checking function, it uses the forwardCheckingUtil
    bool forwardChecking(int m);

    // A recursive utility function to solve m coloring problem using forwardChecking
    bool forwardCheckingUtil(int m, int color[], int v,std::vector<int>* domains);

    /* A function to print the final solution solution */
    void printSolution(int color[]);

};

#endif // _GRAPH_H_
