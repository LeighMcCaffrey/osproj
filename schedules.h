#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <iostream>

#ifndef SCHEDULES_H // include guard
#define SCHEDULES_H

struct Process {
	char id;
	int init_arrival;
	int num_bursts;
	//std::vector<std::tuple<int, int>> burst_times;
	std::vector<std::vector<int> > burst_times;
	double tau;
	int CPU_burst_t_left;
	int IO_burst_t_left;

};
struct Algo_Info {
	double avg_CPU_bt;
	double avg_wait_time;
	double avg_turnaround_time;
	double CPU_utilization;
};


Algo_Info * FCFS(const std::vector<Process* > process_set, int t_cs);
Algo_Info * SJF(const std::vector<Process* > process_set, int t_cs, double alpha );
Algo_Info * SRT(const std::vector<Process* > process_set, int t_cs, double alpha);
Algo_Info * RR(const std::vector<Process* > process_set, int t_cs, int t_slice);

#endif
