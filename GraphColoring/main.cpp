#include <iostream>
#include "local_search.h"


int main(){
    std::string path = "inputs/le450_15a.col";
    Graph g1;
    g1.readGraph(path);
    g1.greedyColoring();
    //std::cout << "100" << std::endl;
    g1.reduceColors();
    //std::cout << "111" << std::endl;


    std::cout << g1.minimalConflicts() << std::endl;

    return 0;

}
