#include "schedules.h"
#include <queue>
#include <deque>
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
struct CompareBurstTimes 
{
    bool operator()(Process * p1, Process * p2)
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        if(p1->tau == p2->tau ) 
        {
            return p1->id < p2->id;
        }
        return p1->tau < p2->tau;
    }
};

struct CompareIOTimeLeft 
{
    bool operator()(Process *p1, Process * p2)
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        if( p1->IO_burst_t_left == p2->IO_burst_t_left ) 
        {
            return p1->id < p2->id;
        }
        return p1->IO_burst_t_left < p2->IO_burst_t_left;
    }
};

/*void update_tau(Process * p, double alpha) 
{
    int burst_index = p->burst_index;
    int CPU_burst_t =  p->burst_times[burst_index][0];
    p->tau = alpha*CPU_burst_t + (1-alpha)*p->tau;
}

void CPU_burst_completion(std::priority_queue<Process *, std::vector<Process *>, CompareBurstTimes> ready_queue,
                        std::priority_queue<Process *, std::vector<Process *>, CompareIOTimeLeft> waiting_state, 
                        Process ** CPU_process_now, Process ** CPU_process_in, Process ** CPU_process_out, int * cs_left, int * num_processes_completed, double alpha, int t_cs) 
{
    if (*CPU_process_now) //*CPU is currently being used by a process
    {
        assert(!*CPU_process_out && !*CPU_process_in);
        if ((*CPU_process_now)->IO_burst_t_left == 0) //*CPU_process_now has completed its burst
        {
            //switch *CPU process out
            *CPU_process_out = *CPU_process_now;
            *CPU_process_now = nullptr;
            assert(*CPU_process_in == nullptr);
            //update tau value for this process //will be availible to the ready queue to use for sorting now
            update_tau(*CPU_process_out, alpha);
        }
    }
    else //CPU is currently not being used by a process, check if the context switch is still occuring (cs_left != 0)
    { 
        if (*cs_left == 0) 
        { 
            if (*CPU_process_in) //introduce new process into CPU burst
            { 
                assert(!*CPU_process_out && !*CPU_process_now);
                //move *CPU_process_in into *CPU_process_now
                *CPU_process_now = *CPU_process_in;
                //Update *CPU_burst time for that process 
                int burst_index = (*CPU_process_now)->burst_index;
                (*CPU_process_now)->CPU_burst_t_left = (*CPU_process_now)->burst_times[burst_index][0];
                *CPU_process_in = nullptr;
            }
                if (*CPU_process_out) //introduce new process into *CPU burst
                {
                    assert(!*CPU_process_in && !*CPU_process_now);
                    //update IO waiting time for that procss and move *CPU_process_out to waiting queue
                    int burst_index = (*CPU_process_out)->burst_index;
                    if ((*CPU_process_out)->burst_times[burst_index][1] != -1) //if it's not the last burst
                    {
                        assert(burst_index < (*CPU_process_out)->num_bursts -1);
                        (*CPU_process_out)->IO_burst_t_left = (*CPU_process_out)->burst_times[burst_index][1];
                        //increment burst_index
                        (*CPU_process_out)->burst_index += 1;
                        //add this process to waiting state
                        waiting_state.push(*CPU_process_out);
                    }
                    else //if it is the last burst
                    {
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
}*/
void CPU_burst_completion2(std::deque<Process * > ready_queue,
                        std::deque<Process * > waiting_state, 
                        Process ** CPU_process_now, Process ** CPU_process_in, Process ** CPU_process_out, int * cs_left, int * num_processes_completed, int t_cs, int t) 
{
    if (*CPU_process_now) //*CPU is currently being used by a process
    {
        //assert(!*CPU_process_out && !*CPU_process_in);
        if ((*CPU_process_now)->IO_burst_t_left == 0) //*CPU_process_now has completed its burst
        {
            //switch *CPU process out
            *CPU_process_out = *CPU_process_now;
            *CPU_process_now = nullptr;
            //assert(*CPU_process_in == nullptr);
            if ( ((*CPU_process_now)->num_bursts) > 0)
            {
                std::cout << "time " << t << "ms: Process " << (*CPU_process_now)->id << " completed a CPU burst; " << (*CPU_process_now)->num_bursts << " to go [Q ";
                if (ready_queue.size() == 0)
                {
                    std::cout << "empty";
                }
                else
                {
                    for (int j = 0; j < ready_queue.size(); j++)
                    {
                        std::cout << ready_queue[j]->id;
                    }
                }
                std::cout << "]\n";
            }
            else
            {
                std::cout << "time " << t << "ms: Process " << (*CPU_process_now)->id << "terminated [Q ";
                if (ready_queue.size() == 0)
                {
                    std::cout << "empty";
                }
                else
                {
                    for (int j = 0; j < ready_queue.size(); j++)
                    {
                        std::cout << ready_queue[j]->id;
                    }
                }
                std::cout << "]\n";
            }

        }
    }
    else //CPU is currently not being used by a process, check if the context switch is still occuring (cs_left != 0)
    { 
        if (*cs_left == 0) 
        { 
            if (*CPU_process_in) //introduce new process into CPU burst
            { 
                //assert(!*CPU_process_out && !*CPU_process_now);
                int burst_index_in = (*CPU_process_in)->burst_index;
                int burst_length_in = (*CPU_process_in)->burst_times[burst_index_in][0];
                char id_in = (*CPU_process_in)->id;
                //move *CPU_process_in into *CPU_process_now
                *CPU_process_now = *CPU_process_in;
                //Update *CPU_burst time for that process 
                int burst_index = (*CPU_process_now)->burst_index;
                (*CPU_process_now)->CPU_burst_t_left = (*CPU_process_now)->burst_times[burst_index][0];
                *CPU_process_in = nullptr;
                std::cout << "time " << t << "ms: Process " << id_in << "started using the CPU for " << burst_length_in << "ms burst [Q ";
                if (ready_queue.size() == 0)
                {
                    std::cout << "empty";
                }
                else
                {
                    for (int j = 0; j < ready_queue.size(); j++)
                    {
                        std::cout << ready_queue[j]->id;
                    }
                }
                std::cout << "]\n";
            }
            if (*CPU_process_out) //introduce new process into *CPU burst
            {
                //assert(!*CPU_process_in && !*CPU_process_now);
                //update IO waiting time for that procss and move *CPU_process_out to waiting queue
                int burst_index = (*CPU_process_out)->burst_index;
                int burst_length = (*CPU_process_out)->burst_times[burst_index][0];
                char id = (*CPU_process_out)->id;
                if ((*CPU_process_out)->burst_times[burst_index][1] != -1) //if it's not the last burst
                {
                    //assert(burst_index < (*CPU_process_out)->num_bursts -1);
                    (*CPU_process_out)->IO_burst_t_left = (*CPU_process_out)->burst_times[burst_index][1];
                    //increment burst_index
                    (*CPU_process_out)->burst_index += 1;
                    //add this process to waiting state
                    waiting_state.push_back(*CPU_process_out);
                }
                else //if it is the last burst
                {
                    //assert(burst_index == (*CPU_process_out)->num_bursts -1);
                    num_processes_completed += 1;
                }
                //if it's the last burst, the process exits without entering the waiting state
                *CPU_process_out = nullptr;
                //if ready queue isn't empty, move new process in
                if(!ready_queue.empty())
                {
                    *CPU_process_in = ready_queue.front();
                    ready_queue.pop_front();
                    * cs_left = t_cs /2;
                }
                int unblock_time = t + burst_length + t_cs;
                std::cout << "time " << t << "ms: Process " << id << " switching out of CPU; will block on I/O until time " << unblock_time << "ms [Q ";
                if (ready_queue.size() == 0)
                {
                    std::cout << "empty";
                }
                else
                {
                    for (int j = 0; j < ready_queue.size(); j++)
                    {
                        std::cout << ready_queue[j]->id;
                    }
                }
                std::cout << "]\n";
            }
        }
    }
}
/*
void IO_burst_completion(std::priority_queue<Process *, std::vector<Process *>, CompareBurstTimes> ready_queue,
                        std::priority_queue<Process *, std::vector<Process *>, CompareIOTimeLeft> waiting_state ) 
{
     while(!waiting_state.empty()) 
     {
        Process * p = waiting_state.top();
        if (p->IO_burst_t_left == 0) 
        {
            ready_queue.push(p);
            waiting_state.pop();
        }
        else //all the processes with zero IO time left will be pushed to the front
        { 
            break;
        }
    }
}*/
void IO_burst_completion2(std::deque<Process * > ready_queue,
                        std::deque<Process * > waiting_state, int t) 
{
     while(!waiting_state.empty()) 
     {
        Process * p = waiting_state.front();
        if (p->IO_burst_t_left == 0) 
        {
            ready_queue.push_back(p);
            waiting_state.pop_front();
            std::cout << "time " << t << "ms: Process " << p->id << " completed I/O; added to ready queue [Q ";
            if (ready_queue.size() == 0)
            {
                std::cout << "empty";
            }
            else
            {
                for (int j = 0; j < ready_queue.size(); j++)
                {
                    std::cout << ready_queue[j]->id;
                }
            }
        }
        else //all the processes with zero IO time left will be pushed to the front
        { 
            break;
        }
    }
}
/*
void new_process_arrivals( std::priority_queue<Process *, std::vector<Process *>, CompareBurstTimes> ready_queue,
                        std::vector<Process* > processes, int t) 
{
    //processes is ordered by arrival time, so once we hit a process with time over current time, we can break loop
    for (int i = 0; i < processes.size(); i++) 
    {
        //add process to ready queue if init time == t
        Process * p = processes[i];
        if (p->init_arrival == t) 
        {
            ready_queue.push(p);
        }
    }
}*/
void new_process_arrivals2(std::deque<Process * > ready_queue,
                        std::vector<Process* > processes, int t) 
{
    //processes is ordered by arrival time, so once we hit a process with time over current time, we can break loop
    for (int i = 0; i < processes.size(); i++) 
    {
        //add process to ready queue if init time == t
        Process * p = processes[i];
        if (p->init_arrival == t) 
        {
            ready_queue.push_back(p);
            std::cout << "time "<< t << "ms: Process " << p->id << " arrived; added to ready queue [Q ";
            for (int j = 0; j < ready_queue.size(); j++)
            {
                std::cout << ready_queue[j]->id;
            }
            std::cout << "]\n";
        }
    }
}
//turnaround time = 
/*Algo_Info * SJF(std::vector<Process* > process_set, int t_cs, double alpha ) 
{
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
        
        
    //think we're going to do the while loop  and increment time each time
    //might want to do while ready_queue isn't empty or somethign
    /*
        (a) CPU burst completion; (b) I/O burst completions (i.e., back to the ready queue); and then (c) new process arrivals.
    
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
        while (!tmp.empty()) 
        {
            Process * p = tmp.top();
            p->IO_burst_t_left -= 1;
            tmp.pop();
        }
        if (CPU_process_now) 
        {
            CPU_process_now->CPU_burst_t_left -=1;
        }
        else 
        {
            assert(CPU_process_in || CPU_process_out);
            cs_left -= 1;
        }
    }

   
}*/
//priority queue
//c
Algo_Info * FCFS(const std::vector<Process* > process_set, int t_cs)
{
    std::cout << "time 0ms: Simulator started for FCFS [Q empty]\n";
    double t;
    std::vector<Process* > processes(process_set);
    std::deque<Process * > ready_queue;
    std::deque<Process * > waiting_state;
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
            //assert(num_processes_completed == n); 
            //all processes have been run
            //TODO- some sort of record keeping
            break;
        }
        //TODO: situation when CPU_proces_now/in/out are all null

        //(a) check CPU burst completion
        CPU_burst_completion2(ready_queue, waiting_state, &CPU_process_now, &CPU_process_in, &CPU_process_out, &cs_left, &num_processes_completed, t_cs, t);
        //(b) I/O burst completions (i.e., back to the ready queue);
        //go through waiting queue and put all processes with IO waiting time 0 into the ready queue
        IO_burst_completion2(ready_queue, waiting_state, t);
        new_process_arrivals2(ready_queue, processes, t);
        //(c) new process arrivals
        //decrement all the counters now/here by one ms
        
        //iterate over waiting_state and decrement all processes' IO_burst_t_left
        std::deque<Process * > tmp = waiting_state;
        while (!tmp.empty()) 
        {
            Process * p = tmp.front();
            p->IO_burst_t_left -= 1;
            tmp.pop_front();
        }
        if (CPU_process_now) 
        {
            CPU_process_now->CPU_burst_t_left -=1;
        }
        else 
        {
            //assert(CPU_process_in || CPU_process_out);
            cs_left -= 1;
        }
    }


}
