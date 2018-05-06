#include <iostream>
#include "graph.h"


int main(){
    std::string path = "inputs/DSJC125.1.col";
    Graph g1;
    g1.readGraph(path);
    g1.printGraph();
    return 0;

}
