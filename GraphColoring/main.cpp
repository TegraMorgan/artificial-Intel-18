#include <iostream>
#include "graph.h"
#define log(x) std::cout <<x << std::endl

int main(){
    std::string path = "inputs/myciel7.col";
    Graph g1;
    g1.readGraph(path);
//    g1.greedyColoring();
//    std::cout << "\nN. of colors: " << g1.getUsedColors().size() << std::endl;
//    std::cout << "Reducing in progress..." << std::endl;
//    int c = g1.reduceColors();
//    std::stringstream sstm;
//    sstm <<"Color " << c << " was reduced.\nValidating...\n";
//    log(sstm.str());
//    //std::cout << "111" << std::endl;
//    int iter = 1;
//    while(g1.minimalConflicts()){
//        std::cout <<iter++ << ". Success" << std::endl;
//        g1.reduceColors();
//    }
//
//
//    //std::cout <<  << std::endl;
    g1.graphColoring(8);
    return 0;

}
