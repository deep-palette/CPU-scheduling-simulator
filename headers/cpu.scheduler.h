



#ifndef __CPU_SCHEDULING_SIM_H_
#define __CPU_SCHEDULING_SIM_H_

/*set the flags of algorithms in order*/

#define FCFS 1
#define SJF 2
#define SRTF 3
#define RR 4
#define Nonpreemptive Priority 5
#define Preemptive Priority 6
#define Nonpreemptive Priority with RR 7

/*structure of simulator*/
typedef struct Simulator {
int flag;
  ProcessPtr now_cpu_burst; /*pointer for current running process*/
  void *ready_queue; /*ptr for ready queue*/
  Priority_Queue *wating_queue; /*ptr for waiting queue*/
  } Simulator;

int Simulator_Init(Simulator **simulator, int flag);
int Simulator_getProcess();
int Simulator_progressTime();
int Simulator_readyQueue();
int Simulator_remainTime();
int Simulator_returnProcess();
int Simulator_startProcess();
int Simulator_TimeSlice();
int Simulator_IncreaseWaitTime();
int Simulator_InsertReadyQueue();
int Simulator_IsFinishedProcess();
