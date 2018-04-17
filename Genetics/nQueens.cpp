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

#include <iostream>					// for cout etc.
#include <vector>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>					// for abs()

#define GA_POPSIZE		1000		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")

#define BOARD_SIZE		8


using namespace std;				// polluting global namespace, but hey...

struct ga_struct
{
	vector<int> sequence;						// the string
	unsigned int fitness;			// its fitness
};

typedef vector<ga_struct> ga_vector;// for brevity

void init_population(ga_vector &population,
	ga_vector &buffer)
{
	int tsize = BOARD_SIZE;

	for (int i = 0; i<GA_POPSIZE; i++) {
		ga_struct citizen;

		citizen.fitness = 0;
		//citizen.sequence.erase();
		citizen.sequence.resize(BOARD_SIZE);

		for (int j = 0; j < tsize; j++)
			citizen.sequence[j] = rand()%BOARD_SIZE;

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}


int maxClashes(int boardSize)
{
	int count = 0;
	for (int i = 1; i < boardSize+1; i++)
		count += i;
	return count;
}

int uniqueLength(vector<int> sequence)
{
	vector<int> temp = sequence;

	sort(temp.begin(),temp.end());
	vector<int>::iterator un = unique(temp.begin(),temp.end());
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

void mutate(ga_struct &member)
{
	int ipos = rand() % BOARD_SIZE;
	int delta = (rand() % BOARD_SIZE);
	
	//member.sequence[ipos] = ((member.sequence[ipos] + delta) % 122);
	
	member.sequence[ipos] = delta;
}

void mate(ga_vector &population, ga_vector &buffer)
{
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = BOARD_SIZE, spos, i1, i2;
	vector<int>::const_iterator first;
	vector<int>::const_iterator last;

	elitism(population, buffer, esize);

	// Mate the rest
	for (int i = esize; i<GA_POPSIZE; i++) {
		i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
		spos = rand() % tsize;

		first = population[i1].sequence.begin();
		last = population[i1].sequence.begin() + spos;
		vector<int> newVec1(first, last);

		first = population[i2].sequence.begin() + spos;
		last = first + (tsize - spos);
		vector<int> newVec2(first, last);

		newVec1.insert(newVec1.end(), newVec2.begin(), newVec2.end());

		buffer[i].sequence = newVec1;

		if (rand() < GA_MUTATION) mutate(buffer[i]);
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
	for (int i = 0; i < GA_POPSIZE; i+=4){
		string os="";
		for (int k = 0; k < BOARD_SIZE; k++){
			os += to_string(g[i].sequence[k]);
		}

		cout << i << ": "<< os << "   " << g[i].fitness << endl;
		
	}
	system("pause");
}

int iterations = 0;

inline void print_best(ga_vector &gav)
{
	string os;

	os += "Best: ";
	for (int i = 0; i < BOARD_SIZE; i++){
		os += to_string(gav[0].sequence[i]);
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
//	
//	for (i = 0; i < 10;i++){
//		cout << "[";
//		for (j = 0; j < BOARD_SIZE-1;j++)
//			cout << pop_alpha[i].sequence[j] << ",";
//		cout << pop_alpha[i].sequence[j] << "]" << endl;
//	}
//
//	cout << uniqueLength(pop_alpha[1].sequence);
//	
//	getchar();
//
//	return 0;
//}
//

int main()
{
	srand(unsigned(time(NULL)));

	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;

	
	for (int i = 0; i<GA_MAXITER; i++) {
		calc_fitness(*population);		// calculate fitness
		sort_by_fitness(*population);	// sort them
		print_best(*population);		// print the best one

		if ((*population)[0].fitness == 0) break;

		mate(*population, *buffer);		// mate the population together
		swap(population, buffer);		// swap buffers
	}
	cout << "End" << endl;
	getchar();

	return 0;
}
