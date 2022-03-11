//FCFS: First COme First Serve

//SJF: Shortest Job First

//SRT: Shortest Remaining Time

//RR: Roubd Robin


float next_exp() {
	//look up how they used formula in exp-random.c
	//flowt next = -log(r)/lambda
	//return next;
}

//make this a class maybe?? I gotta look up format for that
//NOT GOING TO END UP VOID, im just not sure what itll return so placeholder for now
void define_process(char name, seed) {
	//Identify initial process arrival time as floor of next random number in sequence given by next_exp
	int init_arrival = floor(next_exp());
	//Identify # CPU bursts for given process as ceiling of # generated from uniform distribution (use drand48) times 100
		// (int in range [1,100])
	int num_bursts = ceiling(drand48(seed)*100);
	//For each CPu burst, identify CPU burst time and I/O burst time as cieling of next two random numbers given by next_exp
		//mult I/O by 10
	int CPU_time = ceiling(next_exp());
	int IO_time = ceiling(next_exp()) * 10;
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
	float lambda = argv[3]; //constant in exp distribution for interarrival times (avg val 1/lambda, check out exp-random.c)
	int upper = atoi(argv[4]); //upper bound for exp distribution (skip those above the tail like in lab)
	int t_cs = atoi(argv[5]); //Time in ms it takes for a context switch, expect tcs to be positive even int
	float alpha = argv[6]; //For SJF and SRT for estimating burst times
	int t_slice = atoi(argv[7]); //For RR, time slice in ms

	//Define set of processes
	for (int i = 0; i < n; i++) {
		define_process(name, seed);
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