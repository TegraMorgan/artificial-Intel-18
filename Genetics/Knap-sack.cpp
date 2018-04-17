// Genetic5.cpp : Defines the entry point for the console application.
//


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
#define GA_POPSIZE		2048		// ga population size
#define GA_MAXITER		200		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define NUM_OF_ITEMS 15
#define MAX_CAP 750

#define AT 0
using namespace std;				// polluting global namespace, but hey...



struct ga_struct
{
	string items;
	unsigned int weight;
	unsigned int fitness;

};
ga_struct glob;
typedef vector<ga_struct> ga_vector;// for brevity

unsigned int fitness_sum = 0;
unsigned int max_fit = 0;
vector<int> WEIGHTS = {
70,
 73,
 77,
 80,
 82,
 87,
 90,
 94,
 98,
106,
110,
113,
115,
118,
120 };
vector<int> PROFITS = {
  135,
139,
149,
150,
156,
163,
173,
184,
192,
201,
210,
214,
221,
229,
240};
vector<int> big;

void init_population(ga_vector &population,
					 ga_vector &buffer )
{

	for (int i=0; i<GA_POPSIZE; i++) {
		ga_struct citizen;

		citizen.fitness = 0;
		citizen.items.erase();
        citizen.weight = 0;

		for (int j=0; j<NUM_OF_ITEMS; j++)
			citizen.items += (rand() % 2) + '0';

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}

void calc_fitness(ga_vector &population)
{

    //max_fit = 0;
    for(int i = 0; i < GA_POPSIZE; i++){
        unsigned int total_weights = 0,total_profits = 0;
        int num = 0;
        for(int j = 0; j < NUM_OF_ITEMS; j++){
            if(population[i].items[j] == '1'){
                num++;
                total_weights += WEIGHTS[j];
                total_profits += PROFITS[j];
            }
        }
        while(total_weights > MAX_CAP){
            int pos = rand()%NUM_OF_ITEMS;
            if(population[i].items[pos] == '1'){
                num--;
                total_weights -= WEIGHTS[pos];
                total_profits -= PROFITS[pos];
                population[i].items[pos] = '0';
            }
        }
        //total_profits += num;
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
	}

}

void elitism2(ga_vector &population,ga_vector &buffer, int esize ){
	for (int i=GA_POPSIZE - esize; i<GA_POPSIZE; i++) {
		buffer[i].items = population[i].items;
		buffer[i].fitness = population[i].fitness;
		buffer[i].weight = population[i].weight;
	}
}


void mutate(ga_struct &member){
	int ipos = rand() % NUM_OF_ITEMS;
	int delta = (rand() % 2);
	member.items[ipos] = ((member.items[ipos] + delta) % 2) + '0';
}
/*
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
    for(int i = 0; i < GA_TARGET.size(); i++){
        if((double)rand()/RAND_MAX >= ratio){
            result = result + father[i];
        }else{
            result = result + mother[i];
        }
    }
    return result;
}
*/



string uniform_cross_over(string p1, string p2,unsigned int pp1, unsigned int pp2, double ratio){
    string result = "",father,mother;
    if(pp1 > pp2){
        father = p1;
        mother = p2;
    }else{
        father = p2;
        mother = p1;
    }
    for(int i = 0; i < NUM_OF_ITEMS; i++){
        if((double)rand()/RAND_MAX >= ratio){
            result = result + father[i];
        }else{
            result = result + mother[i];
        }
    }
    return result;
}


void mate(ga_vector &population, ga_vector &buffer){

	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = NUM_OF_ITEMS, spos, i1, i2;

	elitism(population , buffer , esize);

	// Mate the rest
	for (int i=esize; i<GA_POPSIZE; i++) {
		i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
		spos = rand() % tsize;

		buffer[i].items = uniform_cross_over( population[i1].items, population[i2].items,population[i1].fitness,population[i2].fitness,0.5 );
        //cout << population[i1].items << "       " << population[i2].items << endl;
		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
}


void mate2(ga_vector &population, ga_vector &buffer){
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = NUM_OF_ITEMS, spos, i1, i2;

	elitism2(population , buffer , esize);

	// Mate the rest
	for (int i=0; i<GA_POPSIZE - esize; i++) {
		i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
		spos = rand() % tsize;

		buffer[i].items = uniform_cross_over( population[i1].items, population[i2].items,population[i1].fitness,population[i2].fitness,0.5 );

		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
}



/*
int roulate_wheel_select(ga_vector& population){
    double p;
    int right,left,mid, i = 0;
    p = (rand() % (int)max_invert_fit);
    p = p/((double)invert_fitness_sum);
    //p /= 1000;

    right = GA_POPSIZE-1;
    left = 0;
    mid = (right + left)/2;

    //printo(population);
    while(right > 2 && left < GA_POPSIZE - 3){
        double pr = (population[mid].invert_fitness/(double)(invert_fitness_sum ));
        double next_pr = (population[mid + 1].invert_fitness/(double)(invert_fitness_sum ));
        if((p <= pr && p >= next_pr)){
            //cout << mid << "  was selected" << endl;
            if(p == next_pr)
                return mid+1;
            return mid;
        }
        else{
            //cout <<  pr << "        "<<  p << endl;
            if(p < pr){
                left = mid +1;
            }
            else{
                right = mid -1;
            }
        }
        mid = (right + left)/2;
//        cout << left << "  " << mid << "  " << right << " #" << i++<< endl
//        << "pr : " << pr << " next pr: " << next_pr<< " p: " << p <<  endl;

    }
    return 0;
}

int roulate(ga_vector& pop){
    double p = (double)(rand()%invert_fitness_sum)/invert_fitness_sum,FitnessSoFar = 0.0;

    for (int i=0; i<GA_POPSIZE; i++){
        FitnessSoFar += pop[i].invert_fitness;

        //if the fitness so far > random number return the chromo at this point
        if (FitnessSoFar >= p)

            return i;
    }

}

void roulate_mate(ga_vector &population, ga_vector &buffer){
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = GA_TARGET.size(), spos, i1, i2;

	elitism(population, buffer, esize);

	// Mate the rest
	for (int i=esize; i<GA_POPSIZE; i++) {

            i1 = roulate_wheel_select(population);
            i2 = roulate_wheel_select(population);
            //cout << i1 << " @@@@ " << i2 << endl;
            spos = rand() % tsize;
            //cout << "222" << endl;
		buffer[i].str = population[i1].str.substr(0, spos) +
			            population[i2].str.substr(spos, tsize - spos);

		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
	        //cout <<endl << endl<< "################################################################################" << endl<<endl;

}
*/


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
    cout << y++ << ": " << "Best: " << gav[AT].items << " (" << gav[AT].fitness << ") " << "(" << MAX_CAP - gav[AT].weight << ") "
                <<"Average: " << fitness_sum/(double)gav.size() << "\nDeviation is: " << dev << endl ;
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



struct myclass {
  bool operator() (int i,int j) { return (i<j);}
} sorter;

bool good_solution(ga_vector &population){
    if(abs(population[0].weight - MAX_CAP) <= 10 && population[0].fitness >= max_fit)
        return true;
    return false;
}
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
    int iterations = 0;
	for (int i=0; i<GA_MAXITER; i++) {
        auto _start_time = std::chrono::high_resolution_clock::now();
        clock_t t_g = clock();
        fitness_sum = 0;
		calc_fitness(*population);		// calculate fitness
        sort_by_fitness(*population);
        double dev = calc_dviation(*population);
		print_best(*population,dev);
				// print the best one
        //p(*population);
        //system("pause");
        //printf("Elasped time: %.2lf seconds.\n\n", difftime(end_generation,begin_generation));
        auto _end_time = std::chrono::high_resolution_clock::now();
        auto _time = _end_time - _start_time;
        cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(_time).count()/1000000.0
                                        <<  " seconds.\n"<< "Clock ticks: " << (float)(clock() - t_g) << endl << endl;
		if (good_solution(*population) && iterations >= GA_MAXITER) break;

		mate2(*population, *buffer);		// mate the population together
		swap(population, buffer);		// swap buffers
	}
    //p(*population);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count()/1000000.0
    <<  " seconds." <<"\nTotal Clock ticks: " << (float)(clock() - t) << endl;
    //system("pause");
	return 0;
}

//int main(){
//    string a = "hello world!", b = "ifmmp xpsme@";
//    cout << two_pts_cross_over(a,b);
//
//}

