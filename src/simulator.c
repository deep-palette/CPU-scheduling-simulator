#include <stdio.h>
#include
#include "../include/DataStructure.h"

#define TIME_QUANTUM 10 /* time quantum value used in RR */
#define CUTTER 10       /* the number of blocks in each line of gantt chart */
int main(){
/*order of input: pid, arrival_ime, burst_time, priority, time_quantum*/
/*order of print: gantt chart, wating_time, average wt, turnaround_time, average tt, response_time, average rt*/


printf("==========================================================================================\n");
printf("[ ID ]   [ARRIVAL_TIME]   [BURST_TIME]   [PRIORITY]   [TIME_QUANTUM]\n");
printf("==========================================================================================\n");


/*open file and get input - > clone them in to simulator*/
  FILE *fp(const char*);

  
  /*Order of scheduler: FCFS, SJF, SRTF, RR, Nonpreemptive priority, preemptive priority, nonprememptive priority with RR*/
  
  
      
  }
