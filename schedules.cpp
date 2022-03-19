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
#include <fstream>
#include <assert.h>
#include <iomanip>

//for breaking ties with ID order
bool ID_sort(Process * p1, Process * p2) {return p1.id<p2.id;}

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

void update_tau(Process * p, double alpha) {
    int burst_index = p->burst_index;
    int CPU_burst_t =  p->burst_times[burst_index][0];
    p->tau = alpha*CPU_burst_t + (1-alpha)*p->tau;
}

void CPU_burst_completion(std::priority_queue<Process *, std::vector<Process *>, CompareBurstTimes> ready_queue,
                        std::priority_queue<Process *, std::vector<Process *>, CompareIOTimeLeft> waiting_state, 
                        Process ** CPU_process_now, Process ** CPU_process_in, Process ** CPU_process_out, int * cs_left, int * num_processes_completed, double alpha, int t_cs) {
        if (*CPU_process_now) { //*CPU is currently being used by a process
            assert(!*CPU_process_out && !*CPU_process_in);

            if ((*CPU_process_now)->CPU_burst_t_left == 0) { //*CPU_process_now has completed its burst
                //switch *CPU process out
                *CPU_process_out = *CPU_process_now;
                *CPU_process_now = nullptr;
                assert(*CPU_process_in == nullptr);
                //update tau value for this process //will be availible to the ready queue to use for sorting now
                update_tau(*CPU_process_out, alpha);
            }
        }
        else { //CPU is currently not being used by a process, check if the context switch is still occuring (cs_left != 0)
            if (*cs_left == 0) { 
                if (*CPU_process_in) { //introduce new process into CPU burst
                    assert(!*CPU_process_out && !*CPU_process_now);
                    //move *CPU_process_in into *CPU_process_now
                    *CPU_process_now = *CPU_process_in;
                    //Update *CPU_burst time for that process 
                    int burst_index = (*CPU_process_now)->burst_index;
                    (*CPU_process_now)->CPU_burst_t_left = (*CPU_process_now)->burst_times[burst_index][0];
                    *CPU_process_in = nullptr;
                }
                if (*CPU_process_out) { //introduce new process into *CPU burst
                    assert(!*CPU_process_in && !*CPU_process_now);
                    //update IO waiting time for that procss and move *CPU_process_out to waiting queue
                    int burst_index = (*CPU_process_out)->burst_index;
                    if ((*CPU_process_out)->burst_times[burst_index][1] != -1) { //if it's not the last burst
                        assert(burst_index < (*CPU_process_out)->num_bursts -1);
                        (*CPU_process_out)->IO_burst_t_left = (*CPU_process_out)->burst_times[burst_index][1];
                        //increment burst_index
                        (*CPU_process_out)->burst_index += 1;
                        //add this process to waiting state
                        waiting_state.push(*CPU_process_out);
                    }
                    else { //if it is the last burst
                        assert(burst_index == (*CPU_process_out)->num_bursts -1);
                        num_processes_completed += 1;

                    }
                    //if it's the last burst, the process exits without entering the waiting state
                    *CPU_process_out = nullptr;
                    //if ready queue isn't empty, move new process in
                    if(!ready_queue.empty()){
                        *CPU_process_in = ready_queue.top();
                        ready_queue.pop();
                        * cs_left = t_cs /2;
                    }
                }
            }
        }
}
void IO_burst_completion(std::priority_queue<Process *, std::vector<Process *>, CompareBurstTimes> ready_queue,
                        std::priority_queue<Process *, std::vector<Process *>, CompareIOTimeLeft> waiting_state ) {
     while(!waiting_state.empty()) {
        Process * p = waiting_state.top();
        if (p->IO_burst_t_left == 0) {
            ready_queue.push(p);
            waiting_state.pop();
        }
        else { //all the processes with zero IO time left will be pushed to the front
            break;
        }
    }
}
std::vector<Process*> new_process_arrivals( std::priority_queue<Process *, std::vector<Process *>, CompareBurstTimes> ready_queue,
                        std::vector<Process* > processes, int t) {
    std::vector<Process*> new_arrivals;
    //processes is ordered by arrival time, so once we hit a process with time over current time, we can break loop
    for (int i = 0; i < processes.size(); i++) {
        //add process to ready queue if init time == t
        Process * p = processes[i];
        if (p->init_arrival == t) {
            ready_queue.push(p);
            new_arrivals.push_back(p);
        }
    }
}

Algo_Info * FCFS(const std::vector<Process* > process_set, int t_cs) {
    std::queue<Process *, std::vector<Process *>> ready_queue
}

//turnaround time = 
Algo_Info * SJF(std::vector<Process* > process_set, int t_cs, double alpha ) {
    double t;
    std::vector<Process* > processes(process_set);
    std::priority_queue<Process *, std::vector<Process *>, CompareBurstTimes> ready_queue;
    std::priority_queue<Process *, std::vector<Process *>, CompareIOTimeLeft> waiting_state;
    Process* CPU_process_now = nullptr;
    Process* CPU_process_in = nullptr;
    Process* CPU_process_out = nullptr;
    //amount of time it takes to remove a process from CPU (and time it takes to add a process )
    //will be used to count down whenever a process is entering and exiting the CPU, otherwise s
    int cs_left = 0 ; // start at 0 so that a new process does its CPU burst
    int n = process_set.size();
    int num_processes_completed = 0;
    //TODO: 
    /*
        - immedietly put new process into ready queue so that the context switch code is ready to take it 
        -make context switch code.
        */
        
    //think we're going to do the while loop  and increment time each time
    //might want to do while ready_queue isn't empty or somethign
    /*
        (a) CPU burst completion; (b) I/O burst completions (i.e., back to the ready queue); and then (c) new process arrivals.
    */
    for (t = 0; true; t++) {

        //check if this is the last process completed
        if (waiting_state.empty() && ready_queue.empty() && processes.empty()
            && CPU_process_in == nullptr && CPU_process_out == nullptr && CPU_process_now == nullptr) {
            assert(num_processes_completed == n); 
            //all processes have been run
            //TODO- some sort of record keeping
            break;
        }
        //TODO: situation when CPU_proces_now/in/out are all null

        //(a) check CPU burst completion
        CPU_burst_completion(ready_queue, waiting_state, &CPU_process_now, &CPU_process_in, &CPU_process_out, &cs_left, &num_processes_completed, alpha, t_cs);
        //(b) I/O burst completions (i.e., back to the ready queue);
        //go through waiting queue and put all processes with IO waiting time 0 into the ready queue
        IO_burst_completion(ready_queue, waiting_state);
        //(c) new process arrivals
        new_process_arrivals(ready_queue, processes, t);
        
         //decrement all the counters now/here by one ms
        
         //iterate over waiting_state and decrement all processes' IO_burst_t_left
         std::priority_queue<Process *, std::vector<Process *>, CompareIOTimeLeft> tmp = waiting_state;
        while (!tmp.empty()) {
            Process * p = tmp.top();
            p->IO_burst_t_left -= 1;
            tmp.pop();
        }
        if (CPU_process_now) {
            CPU_process_now->CPU_burst_t_left -=1;
        }
        else {
            assert(CPU_process_in || CPU_process_out);
            cs_left -= 1;
        }
    }

}

Algo_Info * SRT(const std::vector<Process* > process_set, int t_cs, double alpha) {
    std::priority_queue<Process *, std::vector<Process *>, CompareBurstTimes> ready_queue;
    std::cout << "time 0ms: Simulator started for SRT [Q empty]" << std::endl;
    Process* CPU_process_now = nullptr;
    Process* CPU_process_in = nullptr;
    Process* CPU_process_out = nullptr;
    for (t=0; true; t++) {
        //check if this is the last process completed
        if (waiting_state.empty() && ready_queue.empty() && processes.empty()
            && CPU_process_in == nullptr && CPU_process_out == nullptr && CPU_process_now == nullptr) {
            assert(num_processes_completed == n); 
            std::cout << "time" << t << "ms: Simulator ended for SRT [Q empty]" << std::endl;
            //Print statistics to a file
            //TODO: practice fixed and set precision to make sure this formats correctly
            //TODO: keep track of these values in algo
            /*
            simout << "Algorithm SRT" << std::endl;
            simout << "average CPU burst time: " << std::fixed << std::setprecision(3) << avg_burst << "ms" << std::endl;
            simout << "average wait time: " << std::fixed << std::setprecision(3) << avg_wait << "ms" << std::endl;
            simout << "average turnaround time: " << std::fixed << std::setprecision(3) << avg_turn << "ms" << std::endl;
            simout << "total number of context switches: " << tot_switch << std::endl;
            simout << "total number of preemptions: " << tot_preemp << std::endl;
            simout << "CPU utilization " << std::fixed << std::setprecision(3) << CPU_util << std::endl;
            */

            break;
        }
        // (a) CPU burst completion
        CPU_burst_completion(ready_queue, waiting_state, &CPU_process_now, &CPU_process_in, &CPU_process_out, &cs_left, &num_processes_completed, alpha, t_cs);

        // (b) I/O burst comletion
        IO_burst_completion(ready_queue, waiting_state);

        // (c) new process arrivals
        std::vector<Process*> new_arrivals = new_process_arrivals( ready_queue, process_set, t)
        std::sort(new_arrivals.begin(), new_arrivals.end(),ID_sort);
        //Print all new arrivals, each time printing the complete ready queue
        for (int i = 0; i < new_arrivals.size(); i++) {
            std::cout << "time " << t << "ms: Proccess " << new_arrivals[i]; <<" (tau " << alpha <<"ms)  arrived; added to ready queue [Q";
            for(std::priority_queue<Process *, std::vector<Process *>, CompareBurstTimes> tmp = ready_queue; !tmp.empty(); char p = tmp.pop()) {
                std::cout << " " << p;
            }
            std::cout << "]" << std::endl;
        }
        //iterate over waiting_state and decrement all processes' IO_burst_t_left
        std::priority_queue<Process *, std::vector<Process *>, CompareIOTimeLeft> tmp = waiting_state;
        while (!tmp.empty()) {
            Process * p = tmp.top();
            p->IO_burst_t_left -= 1;
            tmp.pop();
        }
        if (CPU_process_now) {
            CPU_process_now->CPU_burst_t_left -=1;
        }
        else {
            assert(CPU_process_in || CPU_process_out);
            cs_left -= 1;
        }
    }

}

Algo_Info * RR(const std::vector<Process* > process_set, int t_cs, int t_slice) {
    std::queue<Process *, std::vector<Process *>> ready_queue
}