

#ifdef CHECK

int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	return 0;
}

#endif

//#pragma warning(disable:4786)		// disable debug warning

#include <iostream>					// for cout etc.
#include <vector>
#include <array>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>					// for abs()
#include <chrono>
#define GA_POPSIZE		1024		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.7f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")
#define AT 0
#define MAX_AGE 2

using namespace std;



struct ga_struct
{
	string str;						// the string
	unsigned int fitness;			// its fitness
	unsigned int invert_fitness;
	int age;


};
//sum of fitness values
unsigned int fitness_sum = 0;
//maximum fitness value
unsigned int max_fit = 0;
//maximum invert_fitness value
unsigned int max_invert_fit = 0;
//sum of invert_fitness values
unsigned int invert_fitness_sum = 0;


typedef vector<ga_struct> ga_vector;// for brevity


//used for the tournament selection
struct ga_candidate
{
    struct ga_struct gen;
    int i;
};
typedef vector<ga_candidate> ga_t_vector;// for brevity

void init_population(ga_vector &population,ga_vector &buffer ){
	int tsize = GA_TARGET.size();

	for (int i=0; i<GA_POPSIZE; i++) {
		ga_struct citizen;

		citizen.fitness = 0;
		citizen.invert_fitness = 0;
		citizen.str.erase();
        citizen.age = 0;
		for (int j=0; j<tsize; j++)
			citizen.str += (rand() % 90) + 32;

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}

//this function changes the minimization into maximization and calculates the invert_fitness
void calc_maxmize_fitness(ga_vector& population);

//Original fitness function
void calc_fitness(ga_vector &population){
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;
    max_fit = 0;
    fitness_sum = 0;
	for (int i=0; i<GA_POPSIZE; i++) {
		fitness = 0;
		for (int j=0; j<tsize; j++) {
			fitness += abs(int(population[i].str[j] - target[j]));
		}
        if(population[i].age >= MAX_AGE){
            fitness = 5000;
        }
		population[i].fitness = fitness;

		//population[i].invert_fitness = (fitness == 0? 1.0 : 1.0/fitness);
		fitness_sum += fitness;

		if(fitness > max_fit)
            max_fit = fitness;
		//invert_fitness_sum += fitness == 0? 1.0 : 1.0/fitness;
	}
	calc_maxmize_fitness(population);
}

//initialize alphabet array for Bol - Pgea'a
array<char,90> alpha_beta;
bool flag = false;
void initilize_alpha_beta(){
    flag = true;
    string target = GA_TARGET;
	int tsize = target.size();

    for(int i = 0; i < tsize; i++){
        alpha_beta[target[i]%90]++;
    }
}

//Bol Pgea'a - the pseudo code is given in the Report
void calc_b_fitness(ga_vector &population){
    if(!flag)
        initilize_alpha_beta();
    array<char,90> _alpha_beta = alpha_beta;
    array<char,90> __alpha_beta = alpha_beta;
    string target = GA_TARGET;
	int tsize = target.size(), bol = 0,_bol = 0;
	double fitness;
    max_fit = 0;
    fitness_sum = 0;
	for (int i=0; i<GA_POPSIZE; i++) {
        bol = 0;
        _bol = 0;
		fitness = 10*GA_TARGET.size();
		for (int j=0; j<tsize; j++) {
			if(_alpha_beta[population[i].str[j] % 90] > 0){
                if(target[j] == population[i].str[j]){
                    bol++;
                    _alpha_beta[population[i].str[j] % 90]--;
                }else if(__alpha_beta[population[i].str[j] % 90] > 0){
                    _bol++;
                    __alpha_beta[population[i].str[j] % 90]--;
                }

			}else{
                fitness += rand()%tsize*abs(int(population[i].str[j] - target[j]));
			}

		}
		double total;

		if(abs(bol - GA_TARGET.size()) <= 3){
            total = bol;
		}else{
            total = fitness - _bol/2 - bol;
		}

        if(population[i].age >= MAX_AGE){
            total = 10*GA_TARGET.size() + 10*fitness;
        }
		population[i].fitness = total;
		fitness_sum += total;
		if(total > max_fit)
            max_fit = total;
	}
    calc_maxmize_fitness(population);
}

// calculate the opposite fitness: min will become max and so on
void calc_maxmize_fitness(ga_vector &population){
    for(int i = 0 ; i < GA_POPSIZE; i++){
        double k = max_fit - population[i].fitness;
        population[i].invert_fitness = k;
        invert_fitness_sum += k;
        max_invert_fit = (max_invert_fit > k)? max_invert_fit : k;
    }
}

bool fitness_sort(ga_struct x, ga_struct y){
     return (x.fitness < y.fitness);
}

inline void sort_by_fitness(ga_vector &population){
     sort(population.begin(), population.end(), fitness_sort);
}

bool fitness_b_sort(ga_struct x, ga_struct y){
     return (x.fitness < y.fitness);
}

inline void sort_by_b_fitness(ga_vector &population){
     sort(population.begin(), population.end(), fitness_b_sort);
}

void elitism(ga_vector &population,ga_vector &buffer, int esize ){
	for (int i=0; i<esize; i++) {
		buffer[i].str = population[i].str;
		buffer[i].fitness = population[i].fitness;
		buffer[i].invert_fitness = population[i].invert_fitness;
		buffer[i].age = population[i].age + 1;
	}
}

void mutate(ga_struct &member){
	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32;

	member.str[ipos] = ((member.str[ipos] + delta) % 90)+32;
}


//tournament select
int tournament_select(ga_vector& population, int k){
    unsigned int _max = 0;
    int chosen;
    ga_t_vector teams;
    for(int i = 0 ; i < k ; i++){

        int select = rand() % GA_POPSIZE;
        if(population[select].invert_fitness > _max){
            _max = population[select].invert_fitness;
            chosen = i;
        }
    }

    return chosen;
}

string two_pts_cross_over(string p1, string p2){

    int spos1 = 0,spos2 = 0,tsize = GA_TARGET.size();
    spos1 = rand() % (tsize/2);
    spos2 = (rand() % (tsize/2)) + (tsize/2) + 1;

    if(spos2 <= spos1){
        cout << " #########" << endl;
    }

    string result = p1.substr(0,spos1) + p2.substr(spos1,spos2 - spos1) + p1.substr(spos2, tsize - spos2);
    return result;
}

string pt_cross_over(string p1, string p2){
    int spos = 0,tsize = GA_TARGET.size();
    spos = rand() % (tsize);
    string result = p1.substr(0,spos) + p2.substr(spos,tsize - spos);
    return result;
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
    for(int i = 0; i < (int)GA_TARGET.size(); i++){
        if((double)rand()/RAND_MAX >= ratio){
            result = result + father[i];
        }else{
            result = result + mother[i];
        }
    }
    return result;
}

int roulate(ga_vector& pop);

//Identifiers - same as enum
const char TOURNAMENT = 0;
const char ROULETTE = 1;
const char RAND = 2;
const char SINGLE_POINT = 0;
const char TWO_POINTS = 1;
const char UNIFORM = 2;

const char get_select(char a){
    if(a == 0)
        return TOURNAMENT;
    if(a == 1)
        return ROULETTE;
    else
        return RAND;
}

const char get_crossover(char a){
    if(a == 0)
        return SINGLE_POINT;
    if(a == 1)
        return TWO_POINTS;
    else
        return UNIFORM;
}

void mate(ga_vector &population, ga_vector &buffer, int select_cmd , int crossover_cmd){
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = GA_TARGET.size(), spos, i1, i2;

	elitism(population , buffer , esize);

	// Mate the rest
	for (int i=esize; i<GA_POPSIZE; i++) {
        if(select_cmd == TOURNAMENT){
            i1 = tournament_select(population,100);
            i2 = tournament_select(population,100);
        }else if(select_cmd == ROULETTE){
            i1 = roulate(population);
            i2 = roulate(population);
        }else{
            i1 = rand() % (GA_POPSIZE / 2);
            i2 = rand() % (GA_POPSIZE / 2);
        }


		if(crossover_cmd == SINGLE_POINT){
            spos = rand()%tsize;
            buffer[i].str = population[i1].str.substr(0, spos) +
			            population[i2].str.substr(spos, tsize - spos);

		}else if(crossover_cmd == TWO_POINTS){
            buffer[i].str = two_pts_cross_over(population[i1].str,population[i2].str);
		}else{
            buffer[i].str = uniform_cross_over(population[i1].str,population[i2].str,
                                      population[i1].invert_fitness,population[i2].invert_fitness,0.4);
		}

        buffer[i].age = 0;
		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
}


//this function is never used - it's here for future work
//this is an O(log(N)) roulette wheel selection - with a little bug.
//We will fix it later
int roulate_wheel_select(ga_vector& population){
    double p;
    int right,left,mid;
    p = (rand() % (int)max_invert_fit);
    p = sqrt(p/((double)invert_fitness_sum));

    right = GA_POPSIZE-1;
    left = 0;
    mid = (right + left)/2;

    while(right > 1 && left < GA_POPSIZE - 2){
        double pr = sqrt(population[mid].invert_fitness/(double)(invert_fitness_sum ));
        double next_pr = sqrt(population[mid + 1].invert_fitness/(double)(invert_fitness_sum ));
        if((p <= pr && p >= next_pr)){
            if(p == next_pr)
                return mid+1;
            return mid;
        }
        else{
            if(p < pr){
                left = mid +1;
            }
            else{
                right = mid -1;
            }
        }
        mid = (right + left)/2;
    }
    return 0;
}

//Roulette wheel selection method
int roulate(ga_vector& pop){
    double p = sqrt((double)(rand()%invert_fitness_sum));

    int i = 0;
    while( p>=0){
        p -= sqrt((double)pop[i++].invert_fitness);
    }
    return i - 1;

}

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
    cout << y++ << ": " << "Best: " << gav[AT].str << " (" << gav[AT].fitness << ") "
                <<"Average: " << fitness_sum/(double)GA_POPSIZE << "\nDeviation is: " << dev << endl ;
}

inline void swap(ga_vector *&population,ga_vector *&buffer){
     ga_vector *temp = population; population = buffer; buffer = temp;
}




int main()
{
    char selection_method,crossover_method;
    cout << "Genetics Engine - Hello wrold!" << endl << endl
    << "Choose a selection method:\n" << "Tournament Selection: 0\nRoulette Wheel Selection: 1\nRandom Selection: 2\n"
    << "->> ";
    cin >> selection_method;
    cout << endl;
    cout
    << "Choose a crossover method:\n" << "Single Point: 0\nTwo Points: 1\nUniform: 2\n"
    << "->> ";
    cin >> crossover_method;
    cout << endl;
    cout<< "Choose a fitness function:\n" << "Original Fitness: 0\nBull's Eye (Bol-Pgea'a): 1\n"
    << "->> ";
    int f;
    cin >> f;
    cout << endl << endl;
    const char s = get_crossover(selection_method -'0');
    const char c = get_crossover(crossover_method - '0');
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
        if(f == 0){
            calc_fitness(*population);
            sort_by_fitness(*population);
        }else{
            calc_b_fitness(*population);		// calculate fitness
            sort_by_b_fitness(*population);	// sort them
        }

        double dev = calc_dviation(*population);
		print_best(*population,dev);
        auto _end_time = std::chrono::high_resolution_clock::now();
        auto _time = _end_time - _start_time;
        cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(_time).count()/1000000.0
                                        <<  " seconds.\n"<< "Clock ticks: " << (float) ((clock() - t_g)) << endl << endl;
		if ((*population)[AT].str.compare(GA_TARGET) == 0) break;

		mate(*population, *buffer,s,c);		// mate the population together
		swap(population, buffer);		// swap buffers
	}

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0
    <<  " seconds." <<"\nTotal Clock ticks: " << (float)(clock() - t) << endl << endl;
    system("pause");
	return 0;
}


