#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

//FCFS: First COme First Serve

//SJF: Shortest Job First

//SRT: Shortest Remaining Time

//RR: Roubd Robin

struct process {
	int id;
	int init_arrival;
	int num_bursts;
	int ** burst_times;
} process;

float next_exp(float lambda, float long_tail) {
	int iterations = 10000000;
	double sum = 0;
	for ( int i = 0 ; i < iterations ; i++ )
  	{
		/* generate the next pseudo-random value x */
		//double lambda = 0.001;  /* average should be 1/lambda ==> 1000 */
		double r = drand48();   /* uniform dist [0.00,1.00) -- also see random() */
		double x = -log( r ) / lambda;  /* log() is natural log */
		/* skip values that are far down the "long tail" of the distribution */
		if ( x > long_tail ) { i--; continue; }
		sum += x;
  }
  	return sum / iterations;
	//look up how they used formula in exp-random.c
	//flowt next = -log(r)/lambda
	//return next;
}

//TODO: just make process set so that it's pointers to pointers
	//probably avoid issues where you pass in a list and try to mutate it or something
void free_process_set(struct process ** process_set, int n) {
	for (int i = 0; i < n; i ++) {
		free_2D_array(process_set[i]->burst_times, process_set[i]->num_bursts);
		free(process_set[i]);
	}
	free(process_set);
}
//make this a class maybe?? I gotta look up format for that
//NOT GOING TO END UP VOID, im just not sure what itll return so placeholder for now
struct process * define_process(int id, int seed, float lambda, float long_tail ) {
	struct process * p = calloc(1, sizeof(process));
	//Identify initial process arrival time as floor of next random number in sequence given by next_exp
	p->init_arrival = floor(next_exp(lambda, long_tail));
	//Identify # CPU bursts for given process as ceiling of # generated from uniform distribution (use drand48) times 100
		// (int in range [1,100])
	p->num_bursts = ceiling(drand48()*100);
	//For each CPu burst, identify CPU burst time and I/O burst time as cieling of next two random numbers given by next_exp
		//mult I/O by 10
	//creating an array with dimensions (num_bursts x 2)
	// burst_times[i][0] --> CPU_time
	// burst_times[i][1] --> IO_time
	p->burst_times = calloc(p->num_bursts, sizeof(int *));
	for (int i = 0; i < p->num_bursts; i ++) {
		p->burst_times[i] = calloc(2, sizeof(int));
		p->burst_times[i][0] = ceiling(next_exp(lambda, long_tail));
		p->burst_times[i][1] = ceiling(next_exp(lambda, long_tail)) * 10;
	}
	return p;
}

void free_2D_array(int ** array, int length) {
	for (int i = 0; i < length; i ++) {
		free(array[i]);
	}
	free(array);
}

int main(int argc, char** argv) {
	//Get input parameters
	if (argc != 8) {
		fprintf("ERROR: incorrect number of input arguments");
		return EXIT_FAILURE;
	}
	int n = atoi(argv[1]); //Number of processes to simulate, assigned A-Z	
	int seed = atoi(argv[2]); //seed for random numbers, srand48 for each scheduling algo, drand48 to obtain next val in range [0.0,1.0)
	//TODO: remembet what to use for floats instead of atoi
	double lambda = atof(argv[3]); //constant in exp distribution for interarrival times (avg val 1/lambda, check out exp-random.c)
	int upper = atoi(argv[4]); //upper bound for exp distribution (skip those above the tail like in lab)
	int t_cs = atoi(argv[5]); //Time in ms it takes for a context switch, expect tcs to be positive even int
	double alpha = atof(argv[6]); //For SJF and SRT for estimating burst times
	int t_slice = atoi(argv[7]); //For RR, time slice in ms

	//seed the random number generator:
	srand48(seed);
	//Define set of processes
	//values of specific processes can be accessed like "process_set[i]->init_arrival"
	//index within process_set will be the same as id
	struct process ** process_set = calloc(n, sizeof(process));
	for (int i = 0; i < n; i++) {
		process_set[i] = define_process(i, seed, lambda, upper);
	}

	//re-seed random number generator to ensure same processes and times (how to re-seed?)


	/*Ties: if different events occur at same time, handle in this order:
		1) CPU burst completion
		2) IO burst completion
		3) new process arrivals
		any ties within these categories are broken using process ID order (alphabetically)


	When all processes terminate, simulation ends

	Mesurements to keep track of:
		For each algorithm
			-# preemptions
			-# context switches
			-CPU usage
			-CPU idle time
		For each CPU burst
			-CPU burst time (given)
			-turnaround time (note: for last process still include half tcs for removal )
			-wait time
			(all averaged together for each algorithm)
	*/



	return EXIT_SUCCESS;
}