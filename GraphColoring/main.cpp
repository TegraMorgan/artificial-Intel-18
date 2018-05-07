#include <iostream>
#include "graph.h"


int main(){
    std::string path = "inputs/le450_5c.col";
    Graph g1;
    g1.readGraph(path);
    g1.greedyColoring();
    //std::cout << "100" << std::endl;
    g1.reduceColors(0);
    //std::cout << "111" << std::endl;


    //std::cout <<  << std::endl;

    return 0;

}
