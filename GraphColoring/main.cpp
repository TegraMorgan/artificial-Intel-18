#include <iostream>
#include "graph.h"


int main(){
    std::string path = "inputs/myciel6.col";
    Graph g1;
    g1.readGraph(path);
    g1.greedyColoring();
    std::cout << "N. of colors got from greedy coloring: " << g1.getUsedColors().size() << std::endl;
    std::cout << "Reducing in progress..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
/*    g1.reduceColors();
//    std::stringstream sstm;
//    sstm <<"Color " << c << " was reduced.\nValidating...\n";
//    log(sstm.str());
    //std::cout << "111" << std::endl;
    int iter = 1;

    while(g1.minimalConflicts_badEdge()){
        std::cout <<iter++ << ". Success" << std::endl;
        //g1.printSolution(g1.getColors());
        g1.reduceColors();
    }
    if(iter == 1){
        std::cout << "Failure" << std::endl;
    }
    std::cout << std::endl << "Best result: " << g1.getUsedColors().size()+1 << " colors, "
        << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0  << std::endl;
*/
    //std::cout <<  << std::endl;



    long long unsigned sum=0;
    int i = 30;//g1.getUsedColors().size();
    while(g1.graphColoring(i)){
        std::stringstream sstm;
        sstm << "Found coloring with " << (i--) << " colors. " << std::endl << "Number of scanned states: " << g1.states << ".\n\n";
        log(sstm.str());
        sum += g1.states;
        //system("pause");

    }
    sum += g1.states;
    std::cout << std::endl << "Number of scanned states upon FAILURE: " << g1.states << ".\n\n";
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << std::endl << "Best result: " << (i + 1) << " colors.\n"
        << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0  << std::endl
        <<"Total expanded states: " << sum << ".\n" << "Average expanded states for each reduction: " << (sum/( g1.getUsedColors().size() - i +1)) << "\n";

    return 0;
}
