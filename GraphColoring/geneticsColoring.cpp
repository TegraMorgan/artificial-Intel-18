//#include "graph.h"
//#include <iomanip>				// for random seed
//#include <math.h>					// for abs()
//#include <chrono>
//#define GA_POPSIZE		1024		// ga population size
//#define GA_MAXITER		200		// maximum iterations
//#define GA_ELITRATE		0.1f		// elitism rate
//#define GA_MUTATIONRATE	0.9f		// mutation rate
//#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
//#define AT 0
//#define MAX_AGE 2000
//
//
//using namespace std;
//
//
//
//struct ga_struct
//{
//	string items;
//	unsigned int fitness;
//	vector<int> conflicted;
//	int age;
//
//};
//
//Graph* graph;
//int V;
//list<int>* adjacency;
//int* colors;
//typedef vector<ga_struct> ga_vector;// for brevity
//
//
//struct ga_candidate
//{
//    struct ga_struct gen;
//    int i;
//};
//typedef vector<ga_candidate> ga_t_vector;// for brevity
//
//
//
//
//
//
//void init_population(ga_vector &population,ga_vector &buffer ){
//    std::string path = "inputs/le450_5c.col";
//    Graph g;
//    g.readGraph(path);
//    g.greedyColoring();
//    graph = &g;
//    V = g.getV();
//    adjacency = g.getAdjacency();
//    colors = g.getColors();
//    int cs = g.getUsedColors().size();
//    for (int i=0; i<V; i++) {
//		ga_struct citizen;
//
//		citizen.fitness = 0;
//		citizen.items.erase();
//        citizen.age = 0;
//		for (int j=0; j<V; j++){
//            citizen.items += colors[j] + '0';
//            if(j < V-1)
//                citizen.items += ',';
//
//		}
//
//
//		population.push_back(citizen);
//	}
//
//	buffer.resize(GA_POPSIZE);
//}
//
///**
//**
//**/
//void calc_fitness(ga_vector &population){
//    for(int u = 0; u < V; u++){
//        std::list<int>::iterator it;
//        bool isBad = false;
//        int num_of_conflicts = 0;
//        for (it = adjacency[u].begin(); it != adjacency[u].end(); ++it){
//            if(colors[*u] == colors[u]){
//                population.conflicted.push_back(u);
//                break;
//            }
//
//        }
//    }
//
//}
//
//
//
//bool fitness_sort(ga_struct x, ga_struct y){
//     return (x.fitness > y.fitness);
//}
//
//inline void sort_by_fitness(ga_vector &population){
//     sort(population.begin(), population.end(), fitness_sort);
//}
//
//
//void elitism(ga_vector &population,ga_vector &buffer, int esize ){
//
//	for (int i=0; i<esize; i++) {
//		buffer[i].items = population[i].items;
//		buffer[i].fitness = population[i].fitness;
//		buffer[i].weight = population[i].weight;
//		buffer[i].age = population[i].age + 1;
//	}
//
//}
//
//
///**choose a random 0 and change it to 1**/
//
//void mutate(ga_struct &member){
//
//
//}
//
//int tournament_select(ga_vector& population, int k){
//    unsigned int _max = 0;
//    int chosen;
//    ga_t_vector teams;
//    for(int i = 0 ; i < k ; i++){
//
//        int select = rand() % GA_POPSIZE;
//        if(population[select].fitness > _max){
//            _max = population[select].fitness;
//            chosen = i;
//        }
//    }
//
//    return chosen;
//}
//
//
//
//string uniform_cross_over(string p1, string p2,unsigned int pp1, unsigned int pp2, double ratio){
//    string result = "",father,mother;
//    if(pp1 > pp2){
//        father = p1;
//        mother = p2;
//    }else{
//        father = p2;
//        mother = p1;
//    }
//    for(int i = 0; i < num_items; i++){
//        if((double)rand()/RAND_MAX >= ratio){
//            result = result + father[i];
//        }else{
//            result = result + mother[i];
//        }
//    }
//    return result;
//}
//
//
///**
//*The mate method choses the best genome and selects another genome using the tournament and mate them
//* Uniform crossover is used
//*/
//void mate(ga_vector &population, ga_vector &buffer){
//
//	int esize = GA_POPSIZE * GA_ELITRATE;
//	int   i1, i2;
//
//	elitism(population , buffer , esize);
//
//	// Mate the rest
//	for (int i=esize; i<GA_POPSIZE; i++) {
//		i1 = 0;//Best genome in the population
//		i2 = tournament_select(population,100);
//
//
//		buffer[i].items = uniform_cross_over( population[i1].items, population[i2].items,population[i1].fitness,population[i2].fitness,0.5 );
//
//        buffer[i].age = 0;
//		if (rand() < GA_MUTATION) mutate(buffer[i]);
//	}
//}
//
//
//
///**
//**calculates deviation
//**/
//
//inline double calc_dviation(ga_vector& gav){
//    double sum = 0,average = fitness_sum/(double)GA_POPSIZE;
//    for (unsigned int i=0; i<gav.size(); i++) {
//
//        sum += pow(gav[i].fitness - average,2);
//	}
//    sum /= gav.size();
//    return sqrt(sum);
//}
//int y = 0;
//inline void print_best(ga_vector &gav , double dev){
//    cout << y++ << ": " << "Best: " << gav[AT].items << " (" << gav[AT].fitness << ") " << " Vs. Optimal Solution: " << opt
//                <<"\nAverage: " << fitness_sum/(double)gav.size() << "\nDeviation is: " << dev << endl ;
//}
//
//inline void swap(ga_vector *&population,ga_vector *&buffer){
//     ga_vector *temp = population; population = buffer; buffer = temp;
//}
//
//
//
//
//
//
//
//bool sc_1(ga_vector &population){
//    return (population[0].fitness == opt);
//}
//
//bool sc_2(ga_vector &population){
//    if(abs(population[0].weight - max_cap) < get_min() && population[0].fitness >= max_fit)
//        return true;
//    return false;
//}
//
//bool good_solution(ga_vector &population,int cmd){
//    if(cmd == 1)
//        return sc_1(population);
//    return sc_2(population);
//}
//
//void print_problems(){
//    cout << "\n____________________________________" << endl;
//    for(int i = 0; i < 4; i++){
//        CHOOSE = i + 1;
//        init_params();
//        cout << "\n\nProblem "<< (i+1)<<": \nMaximum Capacity: "<< max_cap<<" \nOptimal Profit: "<< opt<<" \nProfits & Weights \n\n";
//        for(int j = 0; j < num_items; j++){
//            cout << "item " << j << ": "<< _PROFITS[j] << " (" << _WEIGHTS[j] << ")" << endl;
//        }
//        cout << "____________________________________";
//    }
//    cout << endl << endl;
//}
//
//int main()
//{
//    cout << "Knap-Sack 0-1 Genetics engine" << endl << endl
//    << "Please choose a problem to solve:";
//    print_problems();
//    cout << "Your choice >> ";
//    cin >> CHOOSE;
//    cout << "Choose a stop condition:\n\n1. Upper Bound\n2. Maximum Profit reached\n";
//    cout << "Your choice >> ";
//    cin >> sc;
//    cout << endl;
//    init_params();
//    clock_t t = clock();
//    auto start_time = std::chrono::high_resolution_clock::now();
//	srand(unsigned(time(NULL)));
//	ga_vector pop_alpha, pop_beta;
//	ga_vector *population, *buffer;
//
//	init_population(pop_alpha, pop_beta);
//	population = &pop_alpha;
//	buffer = &pop_beta;
//
//
//	for (int i=0; i<GA_MAXITER; i++) {
//        auto _start_time = std::chrono::high_resolution_clock::now();
//        clock_t t_g = clock();
//        fitness_sum = 0;
//		calc_fitness(*population);		// calculate fitness
//        sort_by_fitness(*population);
//        double dev = calc_dviation(*population);
//		print_best(*population,dev);
//
//        auto _end_time = std::chrono::high_resolution_clock::now();
//        auto _time = _end_time - _start_time;
//        cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(_time).count()/1000000.0
//                                        <<  " seconds.\n"<< "Clock ticks: " << (float)(clock() - t_g)/CLOCKS_PER_SEC << endl << endl;
//
//		if (good_solution(*population,sc)) break;
//
//		mate(*population, *buffer);		// mate the population together
//		swap(population, buffer);		// swap buffers
//	}
//
//    auto end_time = std::chrono::high_resolution_clock::now();
//    auto time = end_time - start_time;
//    cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0
//    <<  " seconds." <<"\nTotal Clock ticks: " << (float)(clock() - t) << endl;
//
//    system("pause");
//	return 0;
//}
//
