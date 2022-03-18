#include "schedules.h"
#include <queue>
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

//template for comparing processes
struct CompareBurstTimes {
    bool operator()(Process * p1, Process * p2)
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        if(p1->tau == p2->tau ) {
            return p1->id < p2->id;
        }
        return p1->tau < p2->tau;
    }
};

struct CompareIOTimeLeft {
    bool operator()(Process *p1, Process * p2)
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        if( p1->IO_burst_t_left == p2->IO_burst_t_left ) {
            return p1->id < p2->id;
        }
        return p1->IO_burst_t_left < p2->IO_burst_t_left;
    }
};

//TODO comparator with pointers
//turnaround time = 
Algo_Info * SJF(std::vector<Process* > process_set, int t_cs, double alpha ) {
    double t;
    std::vector<Process* > processes(process_set);
    std::priority_queue<Process, std::vector<Process>, CompareBurstTimes> ready_queue;
    std::priority_queue<Process, std::vector<Process>, CompareIOTimeLeft> waiting_state;
    Process* CPU_process_now = nullptr;
    Process* CPU_process_in = nullptr;
    Process* CPU_process_out = nullptr;
    //amount of time it takes to remove a process from CPU (and time it takes to add a process )
    //will be used to count down whenever a process is entering and exiting the CPU, otherwise s
    int cs_left = t_cs /2 ;

    
    //think we're going to do the while loop  and increment time each time
    //might want to do while ready_queue isn't empty or somethign

    /*
        (a) CPU burst completion; (b) I/O burst completions (i.e., back to the ready queue); and then (c) new process arrivals.
    */
    for (t = 0; true; t++) {
        //check CPU burst completion
        if (CPU_process_now) { //CPU is currently being used by the process
            if (CPU_process_now->IO_burst_t_left )

        }

    }
}