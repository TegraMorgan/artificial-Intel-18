#include <iomanip>
#include <iostream>					// for cout etc.
#include <vector>
#include <array>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>					// for abs()
#include <chrono>
#define GA_POPSIZE		1024		// ga population size
#define GA_MAXITER		200		// maximum iterations
#define GA_ELITRATE		0.1f		// elitism rate
#define GA_MUTATIONRATE	0.9f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define AT 0
#define MAX_AGE 2

#define NUM_OF_ITEMS_1 10
#define MAX_CAP_1 165
#define OPTIMAL_PROFIT_1 309

#define NUM_OF_ITEMS_2 7
#define MAX_CAP_2 170
#define OPTIMAL_PROFIT_2 1735

#define NUM_OF_ITEMS_3 6
#define MAX_CAP_3 190
#define OPTIMAL_PROFIT_3 150

#define NUM_OF_ITEMS_4 24
#define MAX_CAP_4 6404180
#define OPTIMAL_PROFIT_4 13549094
using namespace std;


int CHOOSE = 4;
int sc = 1;
struct ga_struct
{
	string items;
	unsigned int weight;
	unsigned int fitness;
	int age;

};
ga_struct glob;
typedef vector<ga_struct> ga_vector;// for brevity


struct ga_candidate
{
    struct ga_struct gen;
    int i;
};
typedef vector<ga_candidate> ga_t_vector;// for brevity

unsigned int fitness_sum = 0;
unsigned int max_fit = 0;

vector<int> WEIGHTS_1 = {23,31,29,44,53,38,63,85,89,82};
vector<int> PROFITS_1 = { 92,57,49,68,60,43,67,84,87,72};

vector<int> WEIGHTS_2 = {41,50,49,59,55,57,60};
vector<int> PROFITS_2 = { 442,525,511,593,546,564,617};

vector<int> WEIGHTS_3 = {56,59,80,64,75,17};
vector<int> PROFITS_3 = {50,50,64,46,50,5};

vector<int> WEIGHTS_4 = {382745,799601,909247,729069,467902,44328,34610,698150,823460,903959,853665,551830,
                                    610856,670702,488960,951111,323046,446298,931161,31385,496951,264724,224916,169684};

vector<int> PROFITS_4 = { 825594,1677009,1676628,1523970,943972,97426,69666,1296457,1679693,1902996,1844992,1049289,
                                    1252836,1319836,953277,2067538,675367,853655,1826027,65731,901489,577243,466257,369261};
//vector<int> big;
vector<int> _WEIGHTS;
vector<int> _PROFITS;
int num_items;
unsigned int max_cap;
unsigned int opt;

void init_params(){
    if(CHOOSE == 1){
        _WEIGHTS = WEIGHTS_1;
        _PROFITS = PROFITS_1;
        num_items = NUM_OF_ITEMS_1;
        max_cap = MAX_CAP_1;
        opt = OPTIMAL_PROFIT_1;

    }else if(CHOOSE == 2){
        _WEIGHTS = WEIGHTS_2;
        _PROFITS = PROFITS_2;
        num_items = NUM_OF_ITEMS_2;
        max_cap = MAX_CAP_2;
        opt = OPTIMAL_PROFIT_2;

    }else if(CHOOSE == 3){
        _WEIGHTS = WEIGHTS_3;
        _PROFITS = PROFITS_3;
        num_items = NUM_OF_ITEMS_3;
        max_cap = MAX_CAP_3;
        opt = OPTIMAL_PROFIT_3;
    }else{
        _WEIGHTS = WEIGHTS_4;
        _PROFITS = PROFITS_4;
        num_items = NUM_OF_ITEMS_4;
        max_cap = MAX_CAP_4;
        opt = OPTIMAL_PROFIT_4;
    }
}


void init_population(ga_vector &population,ga_vector &buffer ){

	for (int i=0; i<GA_POPSIZE; i++) {
		ga_struct citizen;

		citizen.fitness = 0;
		citizen.items.erase();
        citizen.weight = 0;
        citizen.age = 0;
		for (int j=0; j<num_items; j++)
			citizen.items += (rand() % 2) + '0';

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}

/**
** The fitness is the sum of the profits obtained by the sack
** if the genome reaches the maximum age we assigned a fitness of zero
**/
void calc_fitness(ga_vector &population){

    for(int i = 0; i < GA_POPSIZE; i++){
        unsigned int total_weights = 0,total_profits = 0;
        int num = 0;
        for(int j = 0; j < num_items; j++){
            if(population[i].items[j] == '1'){
                num++;
                total_weights += _WEIGHTS[j];
                total_profits += _PROFITS[j];
            }
        }
        while(total_weights > max_cap){
            int pos = rand()%num_items;
            if(population[i].items[pos] == '1'){
                num--;
                total_weights -= _WEIGHTS[pos];
                total_profits -= _PROFITS[pos];
                population[i].items[pos] = '0';
            }
        }

        if(population[i].age >= MAX_AGE){
            total_profits = 0;
        }
        population[i].fitness = total_profits;

        population[i].weight = total_weights;
        fitness_sum += total_profits;
        if(total_profits > max_fit){
            max_fit = total_profits;
            glob = population[i];
        }

    }
}



bool fitness_sort(ga_struct x, ga_struct y){
     return (x.fitness > y.fitness);
}

inline void sort_by_fitness(ga_vector &population){
     sort(population.begin(), population.end(), fitness_sort);
}


void elitism(ga_vector &population,ga_vector &buffer, int esize ){

	for (int i=0; i<esize; i++) {
		buffer[i].items = population[i].items;
		buffer[i].fitness = population[i].fitness;
		buffer[i].weight = population[i].weight;
		buffer[i].age = population[i].age + 1;
	}

}


/**choose a random 0 and change it to 1**/

void mutate(ga_struct &member){

	int ipos = rand() % num_items;
	while(member.items[ipos] == '0'){
        ipos = rand() % num_items;
	}

	member.items[ipos] = '1';
}

int tournament_select(ga_vector& population, int k){
    unsigned int _max = 0;
    int chosen;
    ga_t_vector teams;
    for(int i = 0 ; i < k ; i++){

        int select = rand() % GA_POPSIZE;
        if(population[select].fitness > _max){
            _max = population[select].fitness;
            chosen = i;
        }
    }

    return chosen;
}



string uniform_cross_over(string p1, string p2,unsigned int pp1, unsigned int pp2, double ratio){
    string result = "",father,mother;
    if(pp1 > pp2){
        father = p1;
        mother = p2;
    }else{
        father = p2;
        mother = p1;
    }
    for(int i = 0; i < num_items; i++){
        if((double)rand()/RAND_MAX >= ratio){
            result = result + father[i];
        }else{
            result = result + mother[i];
        }
    }
    return result;
}


/**
*The mate method choses the best genome and selects another genome using the tournament and mate them
* Uniform crossover is used
*/
void mate(ga_vector &population, ga_vector &buffer){

	int esize = GA_POPSIZE * GA_ELITRATE;
	int   i1, i2;

	elitism(population , buffer , esize);

	// Mate the rest
	for (int i=esize; i<GA_POPSIZE; i++) {
		i1 = 0;//Best genome in the population
		i2 = tournament_select(population,100);


		buffer[i].items = uniform_cross_over( population[i1].items, population[i2].items,population[i1].fitness,population[i2].fitness,0.5 );

        buffer[i].age = 0;
		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
}



/**
**calculates deviation
**/

inline double calc_dviation(ga_vector& gav){
    double sum = 0,average = fitness_sum/(double)GA_POPSIZE;
    for (unsigned int i=0; i<gav.size(); i++) {

        sum += pow(gav[i].fitness - average,2);
	}
    sum /= gav.size();
    return sqrt(sum);
}
int y = 0;
inline void print_best(ga_vector &gav , double dev){
    cout << y++ << ": " << "Best: " << gav[AT].items << " (" << gav[AT].fitness << ") " << " Vs. Optimal Solution: " << opt
                <<"\nAverage: " << fitness_sum/(double)gav.size() << "\nDeviation is: " << dev << endl ;
}

inline void swap(ga_vector *&population,ga_vector *&buffer){
     ga_vector *temp = population; population = buffer; buffer = temp;
}

inline void p(ga_vector& g)
{
    for(int i = 0; i < GA_POPSIZE ; i+=4){
        cout << i << "_ " << g[i].items << " : " << g[i].fitness <<  endl;
    }
}

int get_min(){
    int min_ = RAND_MAX;
    for(int i = 0; i < (int)_WEIGHTS.size();i++){
        if(_WEIGHTS[i] < min_){
            min_ = _WEIGHTS[i];
        }
    }
    return min_;
}




bool sc_1(ga_vector &population){
    return (population[0].fitness == opt);
}

bool sc_2(ga_vector &population){
    if(abs(population[0].weight - max_cap) < get_min() && population[0].fitness >= max_fit)
        return true;
    return false;
}

bool good_solution(ga_vector &population,int cmd){
    if(cmd == 1)
        return sc_1(population);
    return sc_2(population);
}

void print_problems(){
    cout << "\n____________________________________" << endl;
    for(int i = 0; i < 4; i++){
        CHOOSE = i + 1;
        init_params();
        cout << "\n\nProblem "<< (i+1)<<": \nMaximum Capacity: "<< max_cap<<" \nOptimal Profit: "<< opt<<" \nProfits & Weights \n\n";
        for(int j = 0; j < num_items; j++){
            cout << "item " << j << ": "<< _PROFITS[j] << " (" << _WEIGHTS[j] << ")" << endl;
        }
        cout << "____________________________________";
    }
    cout << endl << endl;
}

int main()
{
    cout << "Knap-Sack 0-1 Genetics engine" << endl << endl
    << "Please choose a problem to solve:";
    print_problems();
    cout << "Your choice >> ";
    cin >> CHOOSE;
    cout << "Choose a stop condition:\n\n1. Upper Bound\n2. Maximum Profit reached\n";
    cout << "Your choice >> ";
    cin >> sc;
    cout << endl;
    init_params();
    clock_t t = clock();
    auto start_time = std::chrono::high_resolution_clock::now();
	srand(unsigned(time(NULL)));
	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;


	for (int i=0; i<GA_MAXITER; i++) {
        auto _start_time = std::chrono::high_resolution_clock::now();
        clock_t t_g = clock();
        fitness_sum = 0;
		calc_fitness(*population);		// calculate fitness
        sort_by_fitness(*population);
        double dev = calc_dviation(*population);
		print_best(*population,dev);

        auto _end_time = std::chrono::high_resolution_clock::now();
        auto _time = _end_time - _start_time;
        cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(_time).count()/1000000.0
                                        <<  " seconds.\n"<< "Clock ticks: " << (float)(clock() - t_g)/CLOCKS_PER_SEC << endl << endl;

		if (good_solution(*population,sc)) break;

		mate(*population, *buffer);		// mate the population together
		swap(population, buffer);		// swap buffers
	}

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0
    <<  " seconds." <<"\nTotal Clock ticks: " << (float)(clock() - t) << endl;

    system("pause");
	return 0;
}
