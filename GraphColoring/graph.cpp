#include "graph.h"
#include <fstream>
#define LOCAL_SEARCH_1 minimalConflicts()
#define debug(x) std::cout <<x << std::endl

//Add edge to the graph
void Graph::addEdge(int v, int w){
    adj[v].push_back(w);
    if(adj[v].size() > max_adj){
        max_adj = adj[v].size();
    }
    adj[w].push_back(v);
    if(adj[w].size() > max_adj){
        max_adj = adj[w].size();
    }
}


/**
This function used at the first stage of each algorithm, it finds an initial coloring using First-Fit algorithm
It assigns each vertex the minimal color that is not used by it's neighbors
*/
void Graph::greedyColoring(){

    int* result = new int[V];
    // Color first vertex with 0
    result[0]  = 0;
    // Add 0 to the list of used colors
    usedColors.push_back(0);

    // initialize the rest to -1
    for (int u = 1; u < V; u++){
        result[u] = -1;
    }
    // initialize the available array, it's used to indicate if the color at i can be used or not (for each iteration)
    bool* available = new bool[V];
    // Assign all the entries value FALSE
    for (int cr = 0; cr < V; cr++)
        available[cr] = false;


    //traverse over the neighbors of current vertex u
    for (int u = 1; u < V; u++){
        std::list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            //if the neighbor is colored,
            if (result[*i] != -1)
                //mark that colored as used
                available[result[*i]] = true;

        int cr;
        //find minimal available color
        for (cr = 0; cr < V; cr++){
            if (available[cr] == false){
                break;
            }
        }
        //if that color was never assigned before, add it to the list of used colors
        if(std::find(usedColors.begin(), usedColors.end(), cr) == usedColors.end()) {
            usedColors.push_back(cr);
        }
        result[u] = cr; // Assign the found color
        // Reset the values back to false for the next iteration
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = false;
    }
    maxColor = usedColors.size();
    //save the colors
    this -> colors = result;
    delete[] available;
}

/** Utility function taken from www.cplusplus.com - this function is equivalent to the Java function String.split()
    Used in the read graph function
*/
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

/** A function to read the input and construct the graph - trivial*/
void Graph::readGraph(std::string path){
    max_adj = 0;
    std::ifstream file(path);
    std::string str;
    while (std::getline(file, str)){

        if(str[0] != 'e' && str[0] != 'p')
            continue;
        std::vector<std::string> comp  = explode(str,' ');
        if(comp[0] == "c"){
            continue;
        }
        else if(comp[0] == "p"){
            this -> V = std::atoi(comp[2].c_str()) + 1;
            this -> adj = new std::list<int>[this -> V];
        }else if(comp[0] == "e"){
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
int bestF = 0;

/** This function is used in the second part of the homework, it selects random color from the vector usedColors and remove it,
then assign all vertices that were colored with that color a new random color selected from the rest*/
int Graph::reduceColors(){
    srand(unsigned(time(NULL)));
    int reduced_color = usedColors.at(rand() % usedColors.size());

    std::vector<int>::iterator position = std::find(usedColors.begin(), usedColors.end(), reduced_color);
    if (position != usedColors.end()) // == myVector.end() means the element was not found
        usedColors.erase(position);
            //debug("reached");
    int replace_color = usedColors.at(rand() % usedColors.size());
    for(int i = 0; i < V; i++){
        if(this->colors[i] == reduced_color)
            this->colors[i] = replace_color;
    }

    return reduced_color;
}

std::vector<int> Graph::getUsedColors(){
        return this->usedColors;
}


void Graph::print_conflicts(){
    for(int i = 0 ; i < V; i++){
        std::cout << i << ". " << conflicts[i] << std::endl;
    }
}

/** This function fill the conflicts array with conflicts that are in the graph
    Conflict = two adjacent vertices has the same color
    conflicts[i] means the number of adjacent vertices of vertex i that has the same color as his*/
void Graph::find_conflicts(){
    for (int u = 0; u < V; u++){
        std::list<int>::iterator i;
        int num_of_conflicts = 0;
        for (i = adj[u].begin(); i != adj[u].end(); ++i){
            if(colors[*i] == colors[u]){
                num_of_conflicts++;
            }
        }
        conflicts[u] = num_of_conflicts;


    }
}


//COND tells weather all kempe-chains were used
//START tells if this is the first time we search for Kempe-chain
bool COND = true,START = true;

//bi and bj are iterators over the legal pairs of kempe chains
//legal pairs are: (1,2) ... (1,V), (2,3) ... (2,V) , ... ... , (V-1, V)
unsigned int bi = 0, bj = 1;

/** Bad edge objective function, it calculates the expression: Segma(abs(Bi)*abs(Ci)) - Segma(abs(Ci)^2)
    edges = Bi's
    bucket = Ci's
    Segma_C = sum of Ci's (we calculate it before we reach here so no need to calculate it twice)
*/
long int Graph::objective_func_BE(int* edges, int* bucket, unsigned int segma_C ){
    for(unsigned int i = 0 ; i < maxColor; i++){
        edges[i] = 0;
    }
    for(int i = 0 ; i < V; i++){
        std::list<int>::iterator it;
        for(it = adj[i].begin(); it != adj[i].end(); ++it){
            if(*it <= i)
                continue;
            if(colors[*it] == colors[i])
                edges[colors[i]]++;
        }
    }
    long int objective_value = 0;
    for(unsigned int i = 0 ; i < maxColor; i++){
        objective_value += (bucket[i]*edges[i]);
    }
    objective_value *= 2;
    objective_value -= segma_C;
    return objective_value;

}

/**Kempe-Chains objective function to calculate Segma(abs(Ci)^2)
    bucket = Ci's
*/
unsigned int Graph::objective_func_KC(int* bucket){

    for(unsigned int i = 0 ; i < maxColor; i++){
        bucket[i] = 0;
    }
    for(int i = 0 ; i < V; i++){
        bucket[colors[i]]++;
    }
     unsigned int objective_value = 0;
    for(unsigned int i = 0 ; i < maxColor; i++){
        objective_value+= pow(bucket[i],2);
    }
    return objective_value;

}

/**BAD EDGES -
    The algorithm is explained in the report:
    Brief explanation:
    1. find worse vertex
    2. find a color that minimizes the objective function and assign in to it
    3. do this max_iter times
*/

bool Graph::minimalConflicts_badEdge(){
    auto start_time = std::chrono::high_resolution_clock::now();
    int* bucket = new int[maxColor];
    int* edges = new int[maxColor];
    unsigned int max_it = V*100, segma_C = objective_func_KC(bucket);
    long int objective_value = objective_func_BE(edges,bucket,segma_C);
    this->conflicts = new int[V];
    unsigned int i = 0;

    while(i++ < max_it){
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        if(std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0> 500)
            return false;
        find_conflicts();
        int min_idx,max_idx,_min = V,_max = 0;
        std::vector<int> maximals,minimals;
        for(int u = 0; u < V; u++){
            int val = conflicts[u];
            if(val <=_min){
                if(val == _min){
                    minimals.push_back(u);
                }else{
                    minimals.clear();
                    minimals.push_back(u);
                    _min = val;
                }
            }
            if(val >=_max){
                if(val == _max){
                    maximals.push_back(u);
                }else{
                    maximals.clear();
                    maximals.push_back(u);
                    _max = val;
                }
            }
        }
        if( V-minimals.size() == 0){
            START = true;
            bi = 0;
            bj = 1;
            COND = true;
            delete[] bucket;
            delete[] edges;
            return true;
        }


        max_idx = maximals[rand()%maximals.size()];
        min_idx = minimals[rand()%minimals.size()];
        for(unsigned int c = 0; c < usedColors.size(); ++c){
            colors[max_idx] = usedColors[c];
            segma_C = objective_func_KC(bucket);
            long int new_value = objective_func_BE(edges,bucket,segma_C);
            if(new_value < objective_value){
                objective_value = new_value;
                break;
            }
        }
        minimals.clear();
        maximals.clear();
    }
    delete[] bucket;
    delete[] edges;
    return false;
}


/**Kempe-chains -
    The algorithm is explained in the report:
    Brief explanation:
    1. find worse vertex
    2. find a color that minimizes the conflicts and assign it to it
    3. find next kempe-chain
    4. perform swapping for maximum two times if that objective function value wasn't reduced
    5. continue until max_iter
*/
bool Graph::minimalConflicts_kempeChains(){
    //std::cout << "### " << maxColor << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    int* bucket = new int[maxColor];
    int* edges = new int[maxColor];
    unsigned int max_it = V*10, segma_C = objective_func_KC(bucket);
    //long int objective_value = objective_func_BE(edges,bucket,segma_C);
    this->conflicts = new int[V];
    unsigned int i = 0;

    while(i++ < max_it){
            //debug("5");
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        if(std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0> 500)
            return false;
        find_conflicts();
        //debug("6");
        int min_idx,max_idx,_min = V,_max = 0;
        std::vector<int> maximals,minimals;
        for(int u = 0; u < V; u++){
            int val = conflicts[u];
            if(val <=_min){
                if(val == _min){
                    minimals.push_back(u);
                }else{
                    minimals.clear();
                    minimals.push_back(u);
                    _min = val;
                }
            }
            if(val >=_max){
                if(val == _max){
                    maximals.push_back(u);
                }else{
                    maximals.clear();
                    maximals.push_back(u);
                    _max = val;
                }
            }
        }
        //debug("7");
        if( V-minimals.size() == 0){
            START = true;
            bi = 0;
            bj = 1;
            COND = true;
            delete[] bucket;
            delete[] edges;
            return true;
        }


        max_idx = maximals[rand()%maximals.size()];
        min_idx = minimals[rand()%minimals.size()];

        colors[max_idx] = colors[min_idx];
        //std::cout << "3" << std::endl;
        minimals.clear();
        maximals.clear();
        //system("pause");
        if(COND){
            int it = 0;
            while(true){
                if(!COND){
                    break;
                }
                kempe_chains();
                it++;
                unsigned int new_value = objective_func_KC(bucket);
                if(new_value > segma_C){
                    segma_C = new_value;
                    break;
                }
                if(it == 2){
                    break;
                }
            }

        }
    }
    delete[] bucket;
    delete[] edges;
    return false;
}


/**Minimal-Conflicts -
    The algorithm is explained in the report:
    Brief explanation:
    1. find worse vertex
    2. find a color that minimizes the conflicts and assign it to it
    3. continue until max_iter
*/
bool Graph::minimalConflicts(){
    auto start_time = std::chrono::high_resolution_clock::now();
    unsigned int max_it = V*100;
    unsigned int i = 0;
    this->conflicts = new int[V];
    while(i++ < max_it){
            //debug("5");
        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = end_time - start_time;
        if(std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0> 500)
            return false;
        find_conflicts();
        //debug("6");
        int min_idx,max_idx,_min = V,_max = 0;
        std::vector<int> maximals,minimals;
        for(int u = 0; u < V; u++){
            int val = conflicts[u];
            if(val <=_min){
                if(val == _min){
                    minimals.push_back(u);
                }else{
                    minimals.clear();
                    minimals.push_back(u);
                    _min = val;
                }
            }
            if(val >=_max){
                if(val == _max){
                    maximals.push_back(u);
                }else{
                    maximals.clear();
                    maximals.push_back(u);
                    _max = val;
                }
            }
        }
        if( V-minimals.size() == 0){
            return true;
        }


        max_idx = maximals[rand()%maximals.size()];
        min_idx = minimals[rand()%minimals.size()];

        colors[max_idx] = colors[min_idx];
        minimals.clear();
        maximals.clear();

    }
    return false;
}



/** This function return next pair for the kempe chains
    there are n choose 2 different kempe chains
    the next pair is saved in bi & bj
*/
void Graph::get_next_pair(){
    if(bi < usedColors.size()){
        if(bj < usedColors.size()){
            bj++;
            return;
        }else{
            if(bi + 1 == usedColors.size()){
                COND = false;
                return;
            }else{
                bi++;
                bj = bi+1;
            }
            return;
        }
    }else{
        COND = false;
    }
}

void Graph::kempe_chains(){
    //simple trick to avoid increasing bi and bj in the first kempe-chain search
    //bi are initialized to (0,1) - which is the first legal pair
    if(!START){
        get_next_pair();
    }else{
        START = false;
    }
    //if all the kempe chains were swapped , nothing to do, return
    if(!COND){
        return;
    }
    int c1 = usedColors[bi], c2 = usedColors[bj],_max = 0, chain = 0;
    for(int i = 0;i < V; i++){
        int current = colors[i];
        if((current == c1 || current == c2)){
            int counter = 0;
            int cc1 = current == c1? c1 : c2;
            int cc2 = cc1 == c1? c2: c1;

            for(std::list<int>::iterator it = adj[i].begin(); it != adj[i].end(); ++it){
                if(colors[*it] == cc2)
                    counter++;
            }
            if(counter > _max){
                chain = i;
                _max = counter;
            }
        }
    }

    std::vector<int> kempe_chain;
    kempe_chain.push_back(chain);
    for(unsigned int it = 0; it < kempe_chain.size(); ++it){
        int current = colors[kempe_chain[it]];
        int cc1 = current == c1? c1 : c2;
        int cc2 = cc1 == c1? c2: c1;
        for(std::list<int>::iterator it2 = adj[kempe_chain[it]].begin(); it2 != adj[kempe_chain[it]].end(); ++it2){
                if(colors[*it2] == cc2 && (std::find(kempe_chain.begin(),kempe_chain.end(),*it2) == kempe_chain.end())){
                    kempe_chain.push_back(*it2);
                }
        }

    }

    for(std::vector<int>::iterator it = kempe_chain.begin(); it != kempe_chain.end(); ++it){
        colors[*it] = colors[*it] == c1? c2: c1;
    }
    kempe_chain.clear();
    return;
}

bool Graph::isSafe (int v, int color[], int c){
        for (std::list<int>::iterator it = adj[v].begin(); it != adj[v].end(); ++it){
             if (c == color[*it])
                    return false;
        }
        return true;
}

//used to save the problematic vertices
std::vector<int> backJump;
//used to check if problematic vertex was used
std::vector<int> usedJump;

/** Back jumping coloring - it's explained in the report*/
bool Graph::backJumpingColoringUtil(int m, int color[], int v, int usedColor){
    //we set an upper bound to the number of checked states because we found it redundant
    //to proceed if the number reached here
    //the algorithm won't get a better result
    if(states > V*V)
        return false;
    if (v == V)
        return true;
    states++;
    //check if the vertex should be marked for future back jumping
    if(abs(adj[v].size() - max_adj) <= 2  && usedColor == 0 && std::find(usedJump.begin(), usedJump.end(), v) == usedJump.end()){
        backJump.push_back(v);
    }
    int cc = 0;

    for (int c = 1; c <= m; c++){
        if(c == usedColor)
            continue;
        /* Check if assignment of color c to v is fine*/
        if (isSafe(v, color, c) )
        {
           color[v] = c;

           /* recur to assign colors to rest of the vertices */
           if (backJumpingColoringUtil(m, color, v+1, 0) == true){

                return true;
           }

        }
        cc++;

    }

    bool flag = false;
    //if no color is valid, jump back
       while(!backJump.empty()){
            int bac = backJump.back();

            backJump.pop_back();
            usedJump.push_back(bac);
            states--;
            flag = backJumpingColoringUtil(m,color,bac,color[bac]);
            if(flag){
                return true;
            }
       }
       return flag;
    /* If no color can be assigned to this vertex then return false */
    return false;
}

bool Graph::isConflicted(int* cs){

    for (int u = 0; u < V; u++){
        if(cs[u] == 0)
            return true;
        std::list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i){
            if(cs[*i] == cs[u] || cs[*i] == 0){
                return true;
            }
        }


    }
    return false;

}

/** This function used to color the graph with backJumping method*/

bool Graph::backJumpingGraphColoring(int m){
    int *color = new int[V];
    for (int i = 0; i < V; i++)
       color[i] = 0;

    states = 0;
    backJump.clear();
    usedJump.clear();
    if (backJumpingColoringUtil(m, color, 0, 0) == false || isConflicted(color)){

      delete[] color;
      return false;
    }
    delete[] color;
    return true;
}

/* A utility function to print solution */
void Graph::printSolution(int color[]){
    printf("Solution Exists:"
            " Following are the assigned colors \n");
    for (int i = 0; i < V; i++){
        std::cout << "Vertex " << i << " --->  Color "
         << color[i] << std::endl;
         std::list<int>::iterator j;
        for (j = adj[i].begin(); j != adj[i].end();++j){
                std::cout << *j<< "("<< color[*j] << ") , " ;

        }
    }

}

/** Forward-checking coloring - it's explained in the report*/
bool Graph::forwardCheckingUtil(int m, int color[], int v,std::vector<int>* domains){
    if(states > V*V*5)
        return false;
    if( v == V){
        return true;
    }
    states++;
    std::vector<int> backup;
    for(std::vector<int>::iterator it = domains[v].begin(); it != domains[v].end(); ++it){

           color[v] = *it;
           for(std::list<int>::iterator iter = adj[v].begin(); iter != adj[v].end(); ++iter){
                if(color[*iter] != 0)
                    continue;
                std::vector<int>::iterator position = std::find(domains[*iter].begin(), domains[*iter].end(), *it);
                if (position != domains[*iter].end()){ // == myVector.end() means the element was not found
                    domains[*iter].erase(position);
                    backup.push_back(*iter);
                }
           }
           /* recur to assign colors to rest of the vertices */
           if (forwardCheckingUtil(m, color, v+1, domains) == true){

                return true;
           }

            /* If assigning color c doesn't lead to a solution
               return to the state before coloring and return the color to the domains of the neighbors and continue with the next color */
           for(unsigned int u = 0 ; u < backup.size(); u++){
                domains[backup[u]].push_back(*it);
           }
           backup.clear();
           color[v] = 0;

    }

    return false;
}

bool Graph::forwardChecking(int m){
    bool result = false;
    std::vector<int>* domains = new std::vector<int>[V];
    for(int i = 0 ; i < V; i++){
        for(int j = 1 ; j <= m ; j++){
            domains[i].push_back(j);
        }
    }

    int *color = new int[V];
    for (int i = 0; i < V; i++)
       color[i] = 0;

    states = 0;
    if(forwardCheckingUtil(m,color,0,domains)){
        result = true;
    }else{

    }

    delete[] color;
    for(int i = 0 ; i < V ; i++){
        domains[i].clear();
    }
    delete[] domains;
    return result;
}



