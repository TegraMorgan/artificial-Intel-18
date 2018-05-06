#include "graph.h"



void Graph::addEdge(int v, int w){
    adj[v].push_back(w);
    adj[w].push_back(v);  // Note: the graph is undirected
}

// Assigns colors (starting from 0) to all vertices and prints
// the assignment of colors
void Graph::greedyColoring(){
    int* result = new int[V];

    // Assign the first color to first vertex
    result[0]  = 0;

    // Initialize remaining V-1 vertices as unassigned
    for (int u = 1; u < V; u++)
        result[u] = -1;  // no color is assigned to u

    // A temporary array to store the available colors. True
    // value of available[cr] would mean that the color cr is
    // assigned to one of its adjacent vertices
    bool* available = new bool[V];
    for (int cr = 0; cr < V; cr++)
        available[cr] = false;

    // Assign colors to remaining V-1 vertices
    for (int u = 1; u < V; u++)
    {
        // Process all adjacent vertices and flag their colors
        // as unavailable
        std::list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = true;

        // Find the first available color
        int cr;
        for (cr = 0; cr < V; cr++)
            if (available[cr] == false)
                break;

        result[u] = cr; // Assign the found color

        // Reset the values back to false for the next iteration
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = false;
    }

    // print the result
    for (int u = 0; u < V; u++){
        std::cout << "Vertex " << u << " --->  Color "
             << result[u] << std::endl;
    }
    delete[] result;
    delete[] available;
}

const std::vector<std::string> explode(const std::string& s, const char& c){
	std::string buff{""};
	std::vector<std::string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);

	return v;
}

void Graph::readGraph(std::string path){
    std::ifstream file(path);
    std::string str;
    while (std::getline(file, str)){
        std::vector<std::string> comp  = explode(str,' ');
        if(comp[0] == "c"){
            continue;
        }
        else if(comp[0] == "p"){
            this -> V = std::atoi(comp[2].c_str()) + 1;
            this -> adj = new std::list<int>[this -> V];
        }else{
            addEdge(std::atoi(comp[1].c_str()),std::atoi(comp[2].c_str()));
        }

    }
}

void Graph::printGraph(){
    for (int v = 0; v < V; ++v){
        std::cout << "\nAdjacency list of vertex "<< v << ":\n";
        for (auto x : adj[v]){
           std::cout  << x<< " ";
        }
        printf("\n");
    }

}

