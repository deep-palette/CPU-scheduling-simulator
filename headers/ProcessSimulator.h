#pragma once
#include "DataStructure.h"

#define FCFS 1
#define SJF 2
#define SRTF 3
#define RR 4
#define NON_PREEMPTIVE_PRIORITY 5


typedef struct _Simulator {
	unsigned int num_process;              /* the number of processes generated */
	Priority_Queue* generated_processes; /* pointer of job queue */
	Priority_Queue* terminated_processes; /* queue of terminated processes */

	int flag;                        /* flag of scheduling algorithm */
	ProcessPtr cur_cpu_burst;      /* pointer of currently running process */
	void* ready_queue;               /* pointer of ready queue */
	Priority_Queue* waiting_queue; /* pointer of waiting queue */

	int elapsed_time;            /* elapsed time in simulation */
	int idle_time;               /* total amount of cpu idle time */
	double avg_waiting_time;    /* average waiting time */
	double avg_turnaround_time; /* average turnaround time */
	double avg_response_time;
	double max_waiting_time;
} Simulator;

int Simulator_ArrivalTime_Compare(ProcessPtr a, ProcessPtr b);
int initSimulator(Simulator** simulator, int pflag);
int Simulator_GenerateProcess(Simulator** simulators, int* time_quantum);
void Simulator_LoadReadyQueue(Simulator* simulator);
void Simulator_LoadReadyQueue_Priority(Simulator* simulator);
void Simulator_LoadProcess(Simulator* simulator);
void Simulator_LoadProcess_Priority(Simulator* simulator);
void Simulator_LoadProcess_Priority_Preemptive(Simulator* simulator);
void Simulator_Eval(Simulator* simulator);
int Simulator_CPU_Burst(Simulator* simulator);
int Simulator_CPU_Burst_Preemptive(Simulator* simulator);
int Simulator_CPU_Burst_RR(Simulator* simulator, int* time_quantum, int maxtime);
void Simulator_Queue_Waiting(Queue* ready_queue);
void Simulator_Priority_Waiting(TreeNodePtr waiting_process);
void Simulator_FCFS(Simulator* simulator);
void Simulator_SJF(Simulator* simulator);
void Simulator_SRTF(Simulator* simulator);
void Simulator_NonPreemptivePriority(Simulator* simulator);
void Simulator_RR(Simulator* simulator, int maxtime);
void Simulator_Start(Simulator* simulator, int time_quantum);
void Simulator_Terminate(Simulator** simulators);