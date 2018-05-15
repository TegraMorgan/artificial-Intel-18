#include "graph.h"

#define LOCAL_SEARCH_1 minimalConflicts()
#define debug(x) std::cout <<x << std::endl



void Graph::addEdge(int v, int w){
    adj[v].push_back(w);
    if(adj[v].size() > max_adj){
        max_adj = adj[v].size();
    }
    adj[w].push_back(v);  // Note: the graph is undirected
    if(adj[w].size() > max_adj){
        max_adj = adj[w].size();
    }
}

// Assigns colors (starting from 0) to all vertices and prints
// the assignment of colors
void Graph::greedyColoring(){
    int* result = new int[V];
    // Assign the first color to first vertex
    result[0]  = 0;
    usedColors.push_back(0);
    // Initialize remaining V-1 vertices as unassigned
    for (int u = 1; u < V; u++){
        result[u] = -1;
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
    max_adj = 0;
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
        }else if(comp[0] == "e"){
            addEdge(std::atoi(comp[1].c_str()),std::atoi(comp[2].c_str()));
        }

    }
    log("###############\n");
    log(max_adj);
    log("###############\n");
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
    // There's no conflicts

    //std::cout << _min << std::endl;
//    int chosen = minimals[rand()%minimals.size()];
//    minimals.clear();
//    print_conflicts();
//    std::cout << "Min: " << chosen << std::endl;
//    return chosen;

}

//int Graph::find_maximal_conflicted(){
//    //int* colors = this -> g->getColors();
//    int _max = 0,num_zeros = 0;
//
//    std::vector<int> maximals;
//    for (int u = 0; u < V; u++){
//        std::list<int>::iterator i;
//        int num_of_conflicts = 0;
//        for (i = adj[u].begin(); i != adj[u].end(); ++i){
//            if(colors[*i] == colors[u]){
//                num_of_conflicts++;
//            }
//        }
//        conflicts[u] = num_of_conflicts;
//        if(num_of_conflicts > _max){
//            _max = num_of_conflicts;
//            maximals.clear();
//            maximals.push_back(u);
//        }else if(num_of_conflicts == _max){
//            maximals.push_back(u);
//        }
//        if(num_of_conflicts == 0){
//            num_zeros++;
//        }
//    }
//    if(_max == 0){
//        return -1;
//    }
//    //std::cout << _max << std::endl;
//    int chosen = maximals[rand()%maximals.size()];
//    maximals.clear();
//    print_conflicts();
//    std::cout << "Max: " << chosen << std::endl;
//    return chosen;
//}
//

int Graph::get_minimizing_value(int max_idx){
    int result = 0;
    for(std::vector<int>::iterator it = usedColors.begin(); it != usedColors.end(); ++it) {
        int min_conflicts = conflicts[max_idx],num_of_conflicts = 0;
        for (std::list<int>::iterator j = adj[max_idx].begin(); j != adj[max_idx].end(); ++j){
            if(colors[*j] == *it){
                num_of_conflicts++;
            }
        }
        if(num_of_conflicts < min_conflicts){
            min_conflicts = num_of_conflicts;
            result = *it;
        }

    }
    return result;
}

bool COND = true,START = true;
unsigned int bi = 0, bj = 1;

bool Graph::minimalConflicts(){
    //std::cout << "here1" << std::endl;
    unsigned int max_it = V*V/2;
    this->conflicts = new int[V];
    int i = 0;

    while(i++ < max_it){
        find_conflicts();
        int min_idx,max_idx,_min = V,_max = 0;
        std::vector<int> maximals,minimals;
        for(int u = 0; u < V; u++){
            int val = conflicts[u];
//            if(val != 0){
//                terminate = false;
//            }
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
            return true;
        }


        max_idx = maximals[rand()%maximals.size()];
        min_idx = minimals[rand()%minimals.size()];

        //print_conflicts();
        //std::cout << "(Max:Min) " << max_idx << " , " << min_idx << " Total conflicted vertices: " << (V-minimals.size())<< std::endl;

        //std::cout << "1" << std::endl;
        //colors[min_idx] = colors[max_idx];
        //std::cout << "2" << std::endl;
        //int res = get_minimizing_value(max_idx);
        colors[max_idx] = colors[min_idx];
        //std::cout << "3" << std::endl;
        minimals.clear();
        maximals.clear();
        //system("pause");
        if(COND)
            kempe_chains();

    }
    return false;
}




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
    if(!START){
        get_next_pair();
    }else{
        START = false;
    }

    if(!COND){
        //log("\n** All chains were changed **\n");
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
        //std::cout << kempe_chain[it] << " #" << std::endl;
        int current = colors[kempe_chain[it]];
        int cc1 = current == c1? c1 : c2;
        int cc2 = cc1 == c1? c2: c1;
        for(std::list<int>::iterator it2 = adj[kempe_chain[it]].begin(); it2 != adj[kempe_chain[it]].end(); ++it2){
                if(colors[*it2] == cc2 && (std::find(kempe_chain.begin(),kempe_chain.end(),*it2) == kempe_chain.end())){
                    kempe_chain.push_back(*it2);
                }
        }

        //log("\n1\n");
    }
    //log("\n1\n");
    //system("pause");
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

//bool Graph::Inferences(int v, int color[], int m){
//
//        int i, sum=0, index;
//        int* Neighbors_color = new int [m]={0};
//
//        std::list<int>::iterator it;
//        for (it = adj[v].begin(); it != adj[v].end(); ++it){
//            Neighbors_color[color[*it]] = 1;
//        }
//
//        for(i=0;i<;i++){
//            if(Neighbors_color[i])
//                sum++;
//            else
//                index=i;
//        }
//
//        if(sum == (m-1)){
//            color[v] = index;
//            return true;
//        }
//
//}
//

        /* A recursive utility function to solve m coloring problem */
std::vector<int> backJump;
std::vector<int> usedJump;

bool Graph::graphColoringUtil(int m, int color[], int v, int usedColor){

    /* base case: If all vertices are assigned a color then
       return true */
    if (v == V)
        return true;

    if(adj[v].size() == max_adj && usedColor == 0 && std::find(usedJump.begin(), usedJump.end(), v) == usedJump.end()){
        //log("##\nAdded!\n");
        backJump.push_back(v);
    }
    int cc = 0;
    /* Consider this vertex v and try different colors */

    for (int c = 1; c <= m; c++)
    {
        //debug("3");
        if(c == usedColor)
            continue;
        /* Check if assignment of color c to v is fine*/
        if (isSafe(v, color, c) )
        {
           color[v] = c;

           /* recur to assign colors to rest of the vertices */
           if (graphColoringUtil(m, color, v+1, 0) == true){
//                std::stringstream sstm;
//                sstm << v << " " << cc;
//                debug(sstm.str());
                return true;
           }

            /* If assigning color c doesn't lead to a solution
               then remove it */

           color[v] = 0;
           bool flag = false;
           while(!backJump.empty()){
                int bac = backJump.back();

                backJump.pop_back();
                usedJump.push_back(bac);
                flag = graphColoringUtil(m,color,bac,color[bac]);
                if(flag){
//                    std::stringstream sstm;
//                    sstm << "Jumping to vertex: " << bac << " ... SUCCESS!\n";
//                    log(sstm.str());
                    return true;
                }
//                std::stringstream sstm;
//                sstm << "Jumping to vertex: " << bac << " ... FAILURE!\n";
//                log(sstm.str());
           }
           return flag;
        }
        cc++;

    }

    /* If no color can be assigned to this vertex then return false */
    return false;
}

        /* This function solves the m Coloring problem using Backtracking.
      It mainly uses graphColoringUtil() to solve the problem. It returns
      false if the m colors cannot be assigned, otherwise return true and
      prints assignments of colors to all vertices. Please note that there
      may be more than one solutions, this function prints one of the
      feasible solutions.*/
bool Graph::graphColoring(int m){
        // Initialize all color values as 0. This initialization is needed
        // correct functioning of isSafe()
    int *color = new int[V];
    for (int i = 0; i < V; i++)
       color[i] = 0;

    // Call graphColoringUtil() for vertex 0
    if (graphColoringUtil(m, color, 0, 0) == false)
    {
      //printf("Solution does not exist\n");
      //debug("\na");
      delete[] color;
      //debug("b");
      return false;
    }

    // Print the solution
    //printSolution(color);
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
//         std::list<int>::iterator j;
//        for (j = adj[i].begin(); j != adj[i].end();++j){
//                std::cout << color[*j] << " , " ;
//
//        }
//        log("\n");
    }

}

bool Graph::forwardCheckingUtil(int m, int color[], int v,std::vector<int>* domains){
    if( v == V){
        return true;
    }
    std::vector<int> backup;
    for(std::vector<int>::iterator it = domains[v].begin(); it != domains[v].end(); ++it){
        //debug("3");

        /* Check if assignment of color c to v is fine*/
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
//                std::stringstream sstm;
//                sstm << v << " " << cc;
//                debug(sstm.str());
                return true;
           }

            /* If assigning color c doesn't lead to a solution
               then remove it */
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

    if(forwardCheckingUtil(m,color,0,domains)){
        std::cout << "Success" << std::endl;
        result = true;
    }else{
        std::cout << "Failure" << std::endl;
    }
    if(result)
        printSolution(color);
    delete[] color;
    for(int i = 0 ; i < V ; i++){
        domains[i].clear();
    }
    //debug("1");
    delete[] domains;
    return result;
}



