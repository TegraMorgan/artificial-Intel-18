// Genetic5.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#ifdef CHECK

int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	return 0;
}

#endif

#pragma warning(disable:4786)		// disable debug warning
#include <sstream>
#include <iostream>					// for cout etc.
#include <vector>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>					// for abs()
#include <chrono>
#include <array>

#define GA_POPSIZE		1000		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")

#define BOARD_SIZE		8
#define SHUFFLE 3

using namespace std;				// polluting global namespace, but hey...

struct ga_struct
{
	vector<int> sequence;						// the string
	unsigned int fitness;			// its fitness
};

struct ga_candidate
{
    struct ga_struct gen;
    int i;
};
typedef vector<ga_candidate> ga_t_vector;
array<int, BOARD_SIZE> _array;

bool array_init_flag = false;
void init_array()
{

	for (int i = 0; i < BOARD_SIZE; i++)
		_array[i] = 1;
	array_init_flag = true;

}



typedef vector<ga_struct> ga_vector;// for brevity




void init_population(ga_vector &population,
	ga_vector &buffer)
{
	if (!array_init_flag)
		init_array();


	for (int i = 0; ((i<GA_POPSIZE)); i++) {
		array<int, BOARD_SIZE> static_array = _array;
		ga_struct citizen;
		int a;
		citizen.fitness = 0;
		//citizen.sequence.erase();
		citizen.sequence.resize(BOARD_SIZE);

		for (int j = 0; j < BOARD_SIZE; j++){
			//	citizen.sequence[j] = rand() % BOARD_SIZE;


			a = rand() % BOARD_SIZE;
			while (!static_array[a]){
				a = rand() % BOARD_SIZE;
				//cout << "Stuck" << endl;
			}
			citizen.sequence[j] = a;
			static_array[a] = 0;
		}

		population.push_back(citizen);
		//init_array(static_array);
	}

	buffer.resize(GA_POPSIZE);
}


int maxClashes(int boardSize)
{
	int count = 0;
	for (int i = 1; i < boardSize + 1; i++)
		count += i;
	return count;
}

int uniqueLength(vector<int> sequence)
{
	vector<int> temp = sequence;

	sort(temp.begin(), temp.end());
	vector<int>::iterator un = unique(temp.begin(), temp.end());
	temp.resize(distance(temp.begin(), un));

	return(temp.size());
}

int calc_fitness_for_sequence(vector<int> &sequence)
{

	int clashes = 0;
	int rowColumnClashes = abs(int(sequence.size() - uniqueLength(sequence)));
	clashes += rowColumnClashes;
	int dx, dy, i, j;

	// calculate diagonal clashes
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if (i != j){
				dx = abs(i - j);
				dy = abs(sequence[i] - sequence[j]);
				if (dx == dy)
					clashes += 1;
			}
		}
	}

	//return (maxClashes(BOARD_SIZE) - clashes);
	return clashes;

}

void calc_fitness(ga_vector &population)
{

	for (int i = 0; i<GA_POPSIZE; i++) {
		population[i].fitness = calc_fitness_for_sequence(population[i].sequence);
	}
}

bool fitness_sort(ga_struct x, ga_struct y)
{
	return (x.fitness < y.fitness);
}

inline void sort_by_fitness(ga_vector &population)
{
	sort(population.begin(), population.end(), fitness_sort);
}

void elitism(ga_vector &population,
	ga_vector &buffer, int esize)
{
	for (int i = 0; i<esize; i++) {
		buffer[i].sequence = population[i].sequence;
		buffer[i].fitness = population[i].fitness;
	}
}

void mutate1(ga_struct &member)
{
	int ipos = rand() % BOARD_SIZE;
	int delta = rand() % BOARD_SIZE;

	//member.sequence[ipos] = ((member.sequence[ipos] + delta) % 122);

	member.sequence[ipos] = delta;
}

void swap_mutation(ga_struct &member)
{
	int ipos = rand() % BOARD_SIZE;
	int delta = rand() % BOARD_SIZE;
	int temp = member.sequence[ipos];

	member.sequence[ipos] = member.sequence[delta];
	member.sequence[delta] = temp;

}

int myrandom(int i) { return std::rand() % i; }

void scramble_mutation(ga_struct &member)
{
	int _start = 0;
	int _end = -1;
	vector<int>::iterator start_it;
	vector<int>::iterator end_it;

	while (_end <= _start){
		_start = rand() % (BOARD_SIZE / 2);
		_end = (_start + SHUFFLE) % BOARD_SIZE;
	}

	start_it = member.sequence.begin() + _start;
	end_it = member.sequence.begin() + _end;

	random_shuffle(start_it, end_it, myrandom);

}

vector<int> ordered_crossover(vector<int> &parent1, vector<int> &parent2)
{
	if (!array_init_flag)
		init_array();

	array<int, BOARD_SIZE> static_array = _array;
	vector<int> newVec1(BOARD_SIZE, -1);

	int a, k = 0;
	for (int j = 0; j < BOARD_SIZE / 2; j++){
		a = rand() % BOARD_SIZE;
		while (!static_array[a]){
			a = rand() % BOARD_SIZE;
		}
		newVec1[a] = parent1[a];
		static_array[parent1[a]] = 0;
	}

	for (int i = 0; i < BOARD_SIZE; i++){
		if (newVec1[i] == -1){
			while (static_array[parent2[k]] == 0)
				k++;
			newVec1[i] = parent2[k];
			k++;
		}
	}

	return newVec1;
}



vector<int> find_next_cycle(vector<int> &parent1, vector<int> &parent2, int idx){
	int term = parent1[idx];
	vector<int> cycle;
	cycle.push_back(term);
	while (parent2[idx] != term){
		ptrdiff_t pos = find(parent1.begin(), parent1.end(), parent2[idx]) - parent1.begin();
		if (pos <= parent1.size()) {
			cycle.push_back(parent1[pos]);
			idx = pos;
		}
	}
	return cycle;
}

vector<int> cyclic_crossover(vector<int> &parent1, vector<int> &parent2){
	vector<int> p1 = parent1, p2 = parent2, child(BOARD_SIZE, -1), cycle;

	vector<int> so_far(BOARD_SIZE, -1);

	int idx = 0, it = 0;
	while (find(so_far.begin(), so_far.end(), -1) != so_far.end()){
		cycle = find_next_cycle(p1, p2, idx);
		//for (int i = 0; i < cycle.size(); i++){
		//	cout << cycle[i] << " ";
		//}

		for (int i = 0; i < cycle.size(); i++){
			so_far[cycle[i]] = 1;
			ptrdiff_t pos = find(p1.begin(), p1.end(), cycle[i]) - p1.begin();
			if (pos <= p1.size()) {
				child[pos] = cycle[i];

			}
		}
		for (int i = 0; i < p1.size(); i++){
			if (so_far[p1[i]] == -1){
				idx = i;
				break;
			}
		}
		it++;
		if (it % 2 == 0){
			p1 = parent1, p2 = parent2;
		}
		else{
			p1 = parent2, p2 = parent1;
		}
	}

	return child;
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


void mate(ga_vector &population, ga_vector &buffer)
{
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = BOARD_SIZE, spos, i1, i2;
	vector<int> parent1;
	vector<int> parent2;

	elitism(population, buffer, esize);

	// Mate the rest
	for (int i = esize; i<GA_POPSIZE; i++) {
		i1 = tournament_select(population,500); //rand() % (GA_POPSIZE / 2);
		i2 = tournament_select(population,500); //rand() % (GA_POPSIZE / 2);

		parent1 = population[i1].sequence;
		parent2 = population[i2].sequence;

		vector<int> child;

		child = cyclic_crossover(parent1, parent2);

		buffer[i].sequence = child;

		if (rand() < GA_MUTATION) swap_mutation(buffer[i]);
	}
}

void printit(string str){
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			if (str[i] - '0' == j){
				cout << " X";
			}
			else{
				cout << " 0";
			}
		}
		cout << endl;
	}
}

void print_board(ga_vector& g){
	for (int i = 0; i < GA_POPSIZE; i += 4){
		string os = "";
		for (int k = 0; k < BOARD_SIZE; k++){
			os += (g[i].sequence[k] + '0');
		}

		cout << i << ": " << os << "   " << g[i].fitness << endl;

	}
	system("pause");
}

int iterations = 1;

inline void print_best(ga_vector &gav)
{
	string os;

	os += "Best: ";
	for (int i = 0; i < BOARD_SIZE; i++){
		std::ostringstream ss;
		ss << (gav[0].sequence[i]);
		//result += (ss.str() + " ");
		os += (ss.str() + " ");
	}

	cout << iterations++ << ": " << os << " (" << gav[0].fitness << ")" << endl;
	//print_board(gav);
	//printit(os);
}

inline void swap(ga_vector *&population, ga_vector *&buffer)
{
	ga_vector *temp = population; population = buffer; buffer = temp;
}


//int main()
//{
//	srand(unsigned(time(NULL)));
//
//	ga_vector pop_alpha, pop_beta;
//	ga_vector *population, *buffer;
//	int i, j;
//	init_population(pop_alpha, pop_beta);
//	population = &pop_alpha;
//	buffer = &pop_beta;
//	vector<int> newVec1;
//
//	cout << "1: ";
//	for (int i = 0; i < pop_alpha[1].sequence.size(); i++)
//		cout << pop_alpha[1].sequence[i] << " ";
//
//	cout << endl;
//
//	cout << "2: ";
//	for (int i = 0; i < pop_alpha[2].sequence.size(); i++)
//		cout << pop_alpha[2].sequence[i] << " ";
//
//	cout << endl;
//
//	newVec1 = ordered_crossover(pop_alpha[1].sequence, pop_alpha[2].sequence);
//
//
//
//	cout << "3: ";
//	for (int i = 0; i < newVec1.size(); i++)
//		cout << newVec1[i] << " ";
//
//	cout << endl;
//
//	system("pause");
//
//	return 0;
//}

//
//int main()
//{
//	vector<int> newVec1(BOARD_SIZE,-1);
//
//
//	for (int i = 0; i < newVec1.size(); i++)
//		cout << newVec1[i] << " ";
//
//	system("pause");
//	return 0;
//
//}
//

void printBoard(vector<int> toPrint)
{
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			if (toPrint[i] == j){
				cout << " X";
			}
			else{
				cout << " 0";
			}
		}
		cout << endl;
	}
}
//
//int main(){
//	vector<int> p1 = { 4, 1, 7, 6, 2, 8, 3, 5, 0 }, p2 = { 3, 0, 1, 2, 4, 6, 8, 7, 5 }, cycle;
//
//	for (int i = 0; i < p1.size(); i++){
//		cout << p1[i] << " ";
//	}
//	cout << endl;
//	for (int i = 0; i < p2.size(); i++){
//		cout << p2[i] << " ";
//	}
//	cout << endl << endl;
//
//	cycle = cyclic_crossover(p1, p2);
//
//	for (int i = 0; i < cycle.size(); i++){
//		cout << cycle[i] << " ";
//		//so_far[cycle[i]] = 1;
//	}
//
//	system("pause");
//
//	return 0;
//
//}

int main()
{
	clock_t t = clock();
	auto start_time = std::chrono::high_resolution_clock::now();

	srand(unsigned(time(NULL)));

	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;
	bool flag = false;

	cout << "Board Size: " << BOARD_SIZE << endl;
	for (int i = 0; i<GA_MAXITER; i++) {
		auto _start_time = std::chrono::high_resolution_clock::now();
		clock_t t_g = clock();
		calc_fitness(*population);		// calculate fitness
		sort_by_fitness(*population);	// sort them
		print_best(*population);		// print the best one

		auto _end_time = std::chrono::high_resolution_clock::now();
		auto _time = _end_time - _start_time;
		cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(_time).count() / 1000000.0
			<< " seconds.\n" << "Clock ticks: " << (float)(clock() - t_g) << endl << endl;

		if ((*population)[0].fitness == 0){
			flag = true;
			break;
		}

		mate(*population, *buffer);		// mate the population together
		swap(population, buffer);		// swap buffers

	}

	if (flag)
		cout << endl << "Solution: " << endl;
	printBoard((*population)[0].sequence);


	cout << endl;
	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time;
	cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() / 1000000.0
		<< " seconds." << "\nTotal Clock ticks: " << (float)(clock() - t) << endl;
	cout << endl << "End" << endl << endl;

	system("pause");

	return 0;
}
