#include "graph.h"

#define LOCAL_SEARCH_1 minimalConflicts()
#define debug(x) std::cout <<x << std::endl

void Graph::addEdge(int v, int w){
    adj[v].push_back(w);
    adj[w].push_back(v);  // Note: the graph is undirected
}

// Assigns colors (starting from 0) to all vertices and prints
// the assignment of colors
void Graph::greedyColoring(){
    int* result = new int[V];
    int* tmpres = new int[V];
    // Assign the first color to first vertex
    result[0]  = 0;
    usedColors.push_back(0);
    // Initialize remaining V-1 vertices as unassigned
    for (int u = 1; u < V; u++){
        result[u] = -1;
        tmpres[u] = -1;
    }
          // no color is assigned to u

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
        for (cr = 0; cr < V; cr++){
            if (available[cr] == false){
                break;
            }
        }
        if(std::find(usedColors.begin(), usedColors.end(), cr) == usedColors.end()) {
            usedColors.push_back(cr);
        }
        result[u] = cr; // Assign the found color
        tmpres[u] = cr;
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
//    for(int i = 0; i < V; i++){
//        result[i] = i;
//        tmpres[i] = i;
//        usedColors.push_back(i);
//    }
    this -> colors = result;
    this -> backup_colors = tmpres;
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
        std::cout << "\nAdjacency list of vertex "<< v <<" color: "<< this->colors[v] << " Conflicts: " << conflicts[v] << "\n";
        for (auto x : adj[v]){
           std::cout  << colors[x]<< " ";
        }
        printf("\n");
    }

}

int* Graph::getColors(){
    return this -> colors;
}

int Graph::getV(){
    return this->V;
}

std::list<int>* Graph::getAdjacency(){
    return this -> adj;
}

std::string toStr(std::vector<int>& v){
    std::stringstream ss;
    for(size_t i = 0; i < v.size(); ++i)
    {
      if(i != 0)
        ss << ",";
      ss << v[i];
    }
    return ss.str();
}

int glob = 1;
void Graph::reduceColors(int iter){
    int* backup_col = new int[V];
    srand(unsigned(time(NULL)));
    //vector<int> backub_used(usedColors);
    int csize = usedColors.size();
    std::vector<int> used;

    for(int g = 0; g < csize-1; g++){


        for(int c = 0; c < V; c++){
            backup_col[c]=this->colors[c];
        }
        int reduced_color = usedColors.at(rand() % usedColors.size());
        while(std::find(used.begin(), used.end(), reduced_color) != used.end()){
            reduced_color = usedColors.at(rand() % usedColors.size());
        }
        std::cout << "Trying with color: " << reduced_color << " Result: \n";


        int replace_color = usedColors.at(rand() % usedColors.size());
        //std::cout << "4" << std::endl;
        while(replace_color == reduced_color){
            replace_color = usedColors.at(rand() % usedColors.size());
        }

        //std::cout << "5" << std::endl;
        //debug("UN-reached");
//        if(iter > 159){
//            debug(toStr(usedColors));
//        }
        //debug(reduced_color);
        std::vector<int>::iterator position = std::find(usedColors.begin(), usedColors.end(), reduced_color);
        if (position != usedColors.end()) // == myVector.end() means the element was not found
            usedColors.erase(position);
                //debug("reached");
        for(int i = 0; i < V; i++){
            if(this->colors[i] == reduced_color)
                this->colors[i] = replace_color;
        }
        if(LOCAL_SEARCH_1){
            std::cout << "Done " << iter << " colors count: "<<usedColors.size() << std::endl;
            reduceColors(iter+1);
            //printGraph();

        }else{

            glob--;
            std::cout << "Failure" << std::endl << std::endl;
            usedColors.push_back(reduced_color);
            used.push_back(reduced_color);
            for(int c = 0; c < V; c++){
                this->colors[c] = backup_col[c];
            }
        }
    }

    delete[] backup_col;
    debug("\n\n#####DEAD_END######\n\n");

    //std::cout << "6" << std::endl;
}

std::vector<int> Graph::getUsedColors(){
        return this->usedColors;
}



int Graph::find_minimal_conflicted(){
    //int* colors = this -> g->getColors();
    int _min,num_zeros = 0;
    _min = V;
    std::vector<int> minimals;
    for (int u = 0; u < V; u++){
        std::list<int>::iterator i;
        int num_of_conflicts = 0;
        for (i = adj[u].begin(); i != adj[u].end(); ++i){
            if(colors[*i] == colors[u]){
                num_of_conflicts++;
            }
        }
        conflicts[u] = num_of_conflicts;
        if(num_of_conflicts < _min){
            _min = num_of_conflicts;
            minimals.clear();
            minimals.push_back(u);
        }else if(num_of_conflicts == _min){
            minimals.push_back(u);
        }

    }
    // There's no conflicts

    //std::cout << _min << std::endl;
    return minimals[rand()%minimals.size()];

}

int Graph::find_maximal_conflicted(){
    //int* colors = this -> g->getColors();
    int _max = 0,max_idx = 0,num_zeros = 0;

    std::vector<int> maximals;
    for (int u = 0; u < V; u++){
        std::list<int>::iterator i;
        int num_of_conflicts = 0;
        for (i = adj[u].begin(); i != adj[u].end(); ++i){
            if(colors[*i] == colors[u]){
                num_of_conflicts++;
            }
        }
        conflicts[u] = num_of_conflicts;
        if(num_of_conflicts > _max){
            _max = num_of_conflicts;
            maximals.clear();
            maximals.push_back(u);
        }else if(num_of_conflicts == _max){
            maximals.push_back(u);
        }
        if(num_of_conflicts == 0){
            num_zeros++;
        }
    }
    if(_max == 0){
        return -1;
    }
    //std::cout << _max << std::endl;
    return maximals[rand()%maximals.size()];
}




bool Graph::minimalConflicts(){
    //std::cout << "here1" << std::endl;
    this->conflicts = new int[V];
    int i = 0;
    while(i++ < MAX_IT){
            //std::cout << i << std::endl;
        //std::cout << "here2" << std::endl;
        int min_idx = find_minimal_conflicted();
        //std::cout << "here3" << std::endl;

        int max_idx = find_maximal_conflicted();
        if(max_idx == -1){
            //No conflicts found
            return true;
        }
        int tmp_color = colors[min_idx];
        //std::cout << "max conflicts at: " << max_idx << "   %% min conflicts at: " << min_idx << std::endl;
//        if(colors[min_idx] == colors[max_idx]){
//            return false;
//        }
        colors[min_idx] = colors[max_idx];
        colors[max_idx] = tmp_color;
    }
    //printGraph();
    return false;


    //this->g->printGraph();
}

