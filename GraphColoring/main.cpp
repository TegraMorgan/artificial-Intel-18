#include <iostream>
#include<dirent.h>

#include "graph.h"

bool gread = true;
void automation_badEdge(std::string file,std::ofstream* out)
{
    Graph g1;
    g1.readGraph(file);
    g1.greedyColoring();
    if(gread)
        *out << "Greedy: " << g1.getUsedColors().size() << std::endl;

    while(true){
        g1.reduceColors();
        bool result = g1.minimalConflicts_badEdge();
        if(!result)
            break;
    }

    *out << "Best result: " <<  g1.getUsedColors().size()+1 << std::endl;

}

void automation_kempeChains(std::string file,std::ofstream* out)
{

    Graph g1;
    g1.readGraph(file);
    g1.greedyColoring();
    if(gread)
        *out << "Greedy: " << g1.getUsedColors().size() << std::endl;

    while(true){
        g1.reduceColors();
        bool result = g1.minimalConflicts_kempeChains();
        if(!result)
            break;
    }

    *out << "Best result: " <<  g1.getUsedColors().size()+1 << std::endl;

}

void automation_feasible(std::string file,std::ofstream* out)
{
    Graph g1;
    g1.readGraph(file);
    g1.greedyColoring();
    if(gread)
        *out << "Greedy: " << g1.getUsedColors().size() << std::endl;

    while(true){
        g1.reduceColors();
        bool result = g1.minimalConflicts();
        if(!result)
            break;
    }

    *out << "Best result: " <<  g1.getUsedColors().size()+1 << std::endl;

}

void automation_backJumping(std::string file,std::ofstream* out1)
{
    Graph g1;
    g1.readGraph(file);
    g1.greedyColoring();

    if(gread)
        *out1 << "Greedy: " << g1.getUsedColors().size() << std::endl;
    int i =g1.getUsedColors().size();
    long long unsigned sum=0;
    while(true){

        bool result = g1.backJumpingGraphColoring(i);

        sum += g1.states;
        if(!result)
            break;
        i--;
    }

    *out1 << "Best result: "<<  i << "\nOverall number of scanned states:" << sum << std::endl;

}


void automation_forward(std::string file,std::ofstream* out1)
{
    Graph g1;
    g1.readGraph(file);
    g1.greedyColoring();
    *out1 << "Greedy: " << g1.getUsedColors().size() << std::endl;
    int i =g1.getUsedColors().size();
    long long unsigned sum=0;
    while(true){
        bool result = g1.forwardChecking(i);
        sum += g1.states;
        if(!result)
            break;
        i--;
    }

    *out1 << "Best result: "<<  i << "\nOverall number of scanned states:" << sum << std::endl;
}

int main(){
    std::ofstream out,out2;
    out.open("output.txt");
    out2.open("coloring.txt");
    std::cout << "Graph Coloring Engine\n\nMake sure that you have placed you graph inputs in the *INPUT* directory.\nNote that the input format must be like files in the link: http://mat.gsia.cmu.edu/COLOR03.\n\n";
    std::cout <<"Choose your method:\n1. Back-Jumping\n2. Forward-Checking\n3. Local-Search (Feasible)\n4. Local-Search Kempe-Chains\n5. Local-Search Bad Edge\n(Choose a number)--> ";
    int cmd;
    std::cin >> cmd;
    std::cout <<"\nThe graph is being colored, the output will appear in a new txt file named output.txt in this directory.";

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("INPUT/")) != NULL) {
      while ((ent = readdir (dir)) != NULL) {
        if(ent->d_name[0] == '.')
            continue;

        out <<"\n___________________ " <<  ent->d_name << "______________________\n";
        std::cout <<"\nColoring graph: " <<  ent->d_name << "\n";
        std::stringstream sstm;
        sstm << "INPUT/" << ent->d_name;
        std::string path = sstm.str();
        auto start_time = std::chrono::high_resolution_clock::now();
        if(cmd == 1){
            automation_backJumping(path,&out);
        }else if(cmd == 2){
            automation_forward(path,&out);
        }else if(cmd == 3){
            automation_feasible(path,&out);
        }else if(cmd == 4){
            automation_kempeChains(path,&out);
        }else{
            automation_badEdge(path,&out);
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        out << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0 << std::endl;

      }
      closedir (dir);
    } else {
      perror ("");
      return EXIT_FAILURE;
    }

    out.close();
    out2.close();
    system("pause");
    return 0;
}
