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
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")
#define AT 0
#define MAX_AGE 2
//#define AT GA_POPSIZE-1
using namespace std;				// polluting global namespace, but hey...



struct ga_struct
{
	string str;						// the string
	unsigned int fitness;			// its fitness
	unsigned int invert_fitness;
	int age;


};
unsigned int fitness_sum = 0;
unsigned int max_fit = 0;
unsigned int max_invert_fit = 0;

unsigned int invert_fitness_sum = 0;
typedef vector<ga_struct> ga_vector;// for brevity

struct ga_candidate
{
    struct ga_struct gen;
    int i;
};
typedef vector<ga_candidate> ga_t_vector;// for brevity

void init_population(ga_vector &population,
					 ga_vector &buffer )
{
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
unsigned int oo = 0;
void calc_maxmize_fitness(ga_vector& population);
void calc_fitness(ga_vector &population)
{
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
array<char,90> alpha_beta;
//char alpha_beta[90] = {0};
bool flag = false;
void initilize_alpha_beta(){
    flag = true;
    string target = GA_TARGET;
	int tsize = target.size();

    for(int i = 0; i < tsize; i++){
        alpha_beta[target[i]%90]++;
    }
}




void calc_b_fitness(ga_vector &population){
    if(!flag)
        initilize_alpha_beta();
    array<char,90> _alpha_beta = alpha_beta;
    array<char,90> __alpha_beta = alpha_beta;
    //print(_alpha_beta);
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
			//fitness += abs(int(population[i].str[j] - target[j]));
			if(_alpha_beta[population[i].str[j] % 90] > 0){
                if(target[j] == population[i].str[j]){
                    //cout << target[j] << "   " << population[i].str[j] << "  #####" << endl;
                    bol++;
                    //fitness -= bol;

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
            //cout << "WTFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" << endl;
            total = bol;
            //system("pause");
		}else{
            total = fitness - _bol/2 - bol;
		}

        if(population[i].age >= MAX_AGE){
            total = 10*GA_TARGET.size() + 10*fitness;
        }
		population[i].fitness = total;
		//population[i].invert_fitness = 1.0/(fitness == 0? 1 : total);
		fitness_sum += total;
		if(total > max_fit)
            max_fit = total;
		//invert_fitness_sum += 1.0/(fitness == 0? 1 : total);
	}
    calc_maxmize_fitness(population);
}

void calc_maxmize_fitness(ga_vector &population){
    for(int i = 0 ; i < GA_POPSIZE; i++){
        double k = max_fit - population[i].fitness;
        if(k < 0)
            cout <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$4 " << population[i].fitness << "  " << population[i].str << endl;
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
            if(buffer[i].age == MAX_AGE){
                cout << "We're fucked up!";

            }
		buffer[i].str = population[i].str;
		buffer[i].fitness = population[i].fitness;
		buffer[i].invert_fitness = population[i].invert_fitness;
		buffer[i].age = population[i].age + 1;
	}
}

//void elitism2(ga_vector &population,ga_vector &buffer, int esize ){
//	for (int i=GA_POPSIZE - esize; i<GA_POPSIZE; i++) {
//		buffer[i].str = population[i].str;
//		buffer[i].fitness = population[i].fitness;
//		buffer[i].invert_fitness = population[i].invert_fitness;
//		buffer[i].age = population[i].age + 1;
//	}
//}

void mutate(ga_struct &member){
	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32;

	member.str[ipos] = ((member.str[ipos] + delta) % 90)+32;
}

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

void mate(ga_vector &population, ga_vector &buffer){
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = GA_TARGET.size(), spos, i1, i2;

	elitism(population , buffer , esize);

	// Mate the rest
	for (int i=esize; i<GA_POPSIZE; i++) {
		i1 = tournament_select(population,500); //rand() % (GA_POPSIZE / 2);
		i2 = tournament_select(population,500);//rand() % (GA_POPSIZE / 2);
		spos = rand() % tsize;

		buffer[i].str = two_pts_cross_over(population[i1].str,population[i2].str);
		 /*uniform_cross_over(population[i1].str,population[i2].str,
                                      population[i1].invert_fitness,population[i2].invert_fitness,0.4); //population[i1].str.substr(0, spos) +
			            //population[i2].str.substr(spos, tsize - spos);
*/
        buffer[i].age = 0;
		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
}

//void mate2(ga_vector &population, ga_vector &buffer){
//	int esize = GA_POPSIZE * GA_ELITRATE;
//	int tsize = GA_TARGET.size(), spos, i1, i2;
//
//	elitism2(population , buffer , esize);
//
//	// Mate the rest
//	for (int i=0; i<GA_POPSIZE - esize; i++) {
//		i1 = rand() % (GA_POPSIZE / 2);
//		i2 = rand() % (GA_POPSIZE / 2);
//		spos = rand() % tsize;
//
//		buffer[i].str = population[i1].str.substr(0, spos) +
//			            population[i2].str.substr(spos, tsize - spos);
//
//		if (rand() < GA_MUTATION) mutate(buffer[i]);
//	}
//}

void printo(ga_vector& p)
{
    float sum = 0, sum2 = 0;
    for(int i = 0; i < GA_POPSIZE; i++){
        //cout << (1 - p[i].fitness/((double)fitness_sum)) << "    &^&^ " << i<< endl;
//        sum += ( p[i].fitness / fitness_sum);
//        sum2 += 1 - (( p[i].fitness)/fitness_sum );
    cout << p[i].invert_fitness << "  "<<i <<  endl;
    sum+= p[i].invert_fitness;
    }
    cout << "###### ::: " << sum << "    " << invert_fitness_sum << endl;
}

inline void pp(ga_vector& g);

int roulate_wheel_select(ga_vector& population){
    double p;
    int right,left,mid, i = 0;
    p = (rand() % (int)max_invert_fit);
    p = sqrt(p/((double)invert_fitness_sum));
    //p /= 1000;

    right = GA_POPSIZE-1;
    left = 0;
    mid = (right + left)/2;

    //printo(population);
    while(right > 1 && left < GA_POPSIZE - 2){
        double pr = sqrt(population[mid].invert_fitness/(double)(invert_fitness_sum ));
        double next_pr = sqrt(population[mid + 1].invert_fitness/(double)(invert_fitness_sum ));
        if((p <= pr && p >= next_pr)){
            //cout << mid << "  was selected" << endl;
            if(population[mid].age > MAX_AGE){
                //pp(population);
                system("pause");
            }
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
    double p = sqrt((double)(rand()%invert_fitness_sum)),FitnessSoFar = 0.0;

//    for (int i=0; i<GA_POPSIZE; i++){
//        FitnessSoFar += sqrt(pop[i].invert_fitness);
//
//        //if the fitness so far > random number return the chromo at this point
//        if (FitnessSoFar >= p)
//
//            return i;
//    }
    int i = 0;
    while( p>=0){
        p -= sqrt((double)pop[i++].invert_fitness);
    }
    return i - 1;

}

void roulate_mate(ga_vector &population, ga_vector &buffer){
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = GA_TARGET.size(), spos, i1, i2;

	elitism(population, buffer, esize);

	// Mate the rest
	for (int i=esize; i<GA_POPSIZE; i++) {

            i1 = roulate(population);
            i2 = roulate(population);
            //cout << i1 << " @@@@ " << i2 << endl;
            spos = rand() % tsize;
            //cout << "222" << endl;
		buffer[i].str = uniform_cross_over(population[i1].str,population[i2].str,
                                      population[i1].invert_fitness,population[i2].invert_fitness,0.4); //population[i1].str.substr(0, spos) +
        buffer[i].age = 0;
		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
	        //cout <<endl << endl<< "################################################################################" << endl<<endl;

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
    cout << y++ << ": " << "Best: " << gav[AT].str << " (" << gav[AT].fitness << ") " << "[ " << gav[AT].age << "] "
                <<"Average: " << fitness_sum/(double)GA_POPSIZE << "\nDeviation is: " << dev << endl ;
}

inline void swap(ga_vector *&population,ga_vector *&buffer){
     ga_vector *temp = population; population = buffer; buffer = temp;
}

inline void pp(ga_vector& g)
{
    for(int i = 0; i < GA_POPSIZE ; i+=4){
        cout << i << "_ " << g[i].str <<"  " << g[i].fitness << endl;
    }
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

	for (int i=0; i<GA_MAXITER; i++) {
        auto _start_time = std::chrono::high_resolution_clock::now();
        clock_t t_g = clock();
        fitness_sum = 0;
		calc_b_fitness(*population);		// calculate fitness
		sort_by_b_fitness(*population);	// sort them
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
		if ((*population)[AT].str.compare(GA_TARGET) == 0) break;

		mate(*population, *buffer);		// mate the population together
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
