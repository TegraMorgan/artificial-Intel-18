#include <iostream>
#include "graph.h"


int main(){
    std::string path = "inputs/queen13_13.col";
    Graph g1;
    g1.readGraph(path);
    g1.greedyColoring();
    std::cout << "\nN. of colors: " << g1.getUsedColors().size() << std::endl;
    std::cout << "Reducing in progress..." << std::endl;
    int c = g1.reduceColors();
    std::stringstream sstm;
    sstm <<"Color " << c << " was reduced.\nValidating...\n";
    log(sstm.str());
    //std::cout << "111" << std::endl;
    int iter = 1;
    while(g1.minimalConflicts()){
        std::cout <<iter++ << ". Success" << std::endl;
        g1.reduceColors();
    }
    if(iter == 1){
        std::cout << "Failure" << std::endl;
    }

    std::cout << "Best result: " << g1.getUsedColors().size()+1 << " colors." << std::endl;

    //std::cout <<  << std::endl;



    /*
    int i = 1;
    if(g1.forwardChecking(i)){
        std::stringstream sstm;
        sstm << "Found coloring with " << (i) << " colors. " << std::endl;
        log(sstm.str());

    }
    //std::cout << "Best solution could be reached: " << (i) << " Colors." << std::endl;*/
    return 0;
}
