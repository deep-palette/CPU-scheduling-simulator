#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "DataStructure.h"
#include "ProcessSimulator.h"

int Simulator_ArrivalTime_Compare(ProcessPtr a, ProcessPtr b) {
    int first = a->arrival_time, f_id = a->pid;
    int second = b->arrival_time, s_id = b->pid;

    return first != second ? first < second : f_id < s_id;
}

int initSimulator(Simulator** simulator, int pflag) {
    *simulator = (Simulator*)malloc(sizeof(Simulator));

    if (*simulator == NULL) exit(-1);

    (*simulator)->num_process = 0;
    (*simulator)->cur_cpu_burst = NULL;
    (*simulator)->elapsed_time = 0;
    (*simulator)->avg_waiting_time = 0;
    (*simulator)->avg_turnaround_time = 0;
    (*simulator)->avg_response_time = 0;
    (*simulator)->max_waiting_time = 0;

    switch (pflag) {
    case FCFS:
    case RR:
        Queue_Init((Queue**)&((*simulator)->ready_queue));
        break;
    case SJF:
    case SRTF:
        Priority_Init((Priority_Queue**)&((*simulator)->ready_queue), SJF_Compare);
        break;
    case NON_PREEMPTIVE_PRIORITY:
        Priority_Init((Priority_Queue**)&((*simulator)->ready_queue), Priority_Compare);
        break;
    default:
        printf("Error\n");
        exit(-1);
    }

    /* Allocate memory for waiting queue and initialize */
    Priority_Init(&(*simulator)->waiting_queue, Simulator_ArrivalTime_Compare);

    /* Allocate memory for generated processes and initialize */
    Priority_Init(&(*simulator)->generated_processes, Simulator_ArrivalTime_Compare);

    /* Allocate memory for queue of terminated processes and initialize */
    Priority_Init(&(*simulator)->terminated_processes, Simulator_ArrivalTime_Compare);

    /* Set scheduling algorithm flag */
    (*simulator)->flag = pflag;

    return 1;
}

int Simulator_GenerateProcess(Simulator** simulators, int* time_quantum) {
    unsigned char data[500]; /* array of random cpu burst time values */
    ProcessPtr new;        /* pointer of newly generated process */
    char ch;
    int N, t;
    int pid[100];
    int arrival_time[100];
    int burst_time[100];
    int priority[100];

    FILE* fp;
    fp = fopen("Input.txt", "r");
    fscanf(fp, "%d\n", &N);

    for (int i = 0; simulators[i]; i++)
        simulators[i]->num_process = N;

    for (int i = 0; i < N; i++) { // 입력 완료
        fscanf(fp, "%c%d %d %d %d\n", &ch, &pid[i], &arrival_time[i], &burst_time[i], &priority[i]);

        for (int j = 0; simulators[j]; j++) {
            new = (ProcessPtr)malloc(sizeof(Process));

            if (new == NULL) {
                printf("Error\n");
                exit(-1);
            }

            new->pid = pid[i];
            new->arrival_time = arrival_time[i];
            new->burst_time = burst_time[i];
            new->priority = priority[i];
            new->isResponsed = 0;
            new->waiting_time = 0;
            new->turnaround_time = 0;
            new->response_time = 0;

            Priority_Enqueue(simulators[j]->generated_processes, new);
        }
    }
    fscanf(fp, "%d", &t);
    *time_quantum = t;
    fclose(fp);

    // 출력
    printf(
        "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        "++++++++++"
        "\n");
    printf(
        "++   PID   ++  CPU_BURST_TIME ++  ARRIVAL_TIME  ++  "
        "PRIORITY  "
        "++\n");
    printf(
        "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        "++++++++++"
        "\n");

    for (int i = 0; i < N; i++) {
        printf("++  %5d  ++  %13d  ++  %12d  ++  %8d  ++\n", pid[i],
            burst_time[i], arrival_time[i], priority[i]);
    }
    printf(
        "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        "++++++++++++\n");

    return 1;
}

void Simulator_LoadReadyQueue(Simulator* simulator) {
    Queue* ready_queue = simulator->ready_queue;
    Priority_Queue* waiting_queue = simulator->waiting_queue;
    Priority_Queue* job_queue = simulator->generated_processes;
    Priority_Queue* terminated_processes = simulator->terminated_processes;
    ProcessPtr front_process;


    while (!Priority_IsEmpty(job_queue) &&
        (front_process = Priority_Top(job_queue))->arrival_time ==
        simulator->elapsed_time) {
        Queue_Enqueue(ready_queue, front_process);
        Priority_Dequeue(job_queue);
    }



    return;
}

void Simulator_LoadReadyQueue_Priority(Simulator* simulator) {
    Priority_Queue* ready_queue = simulator->ready_queue;
    Priority_Queue* waiting_queue = simulator->waiting_queue;
    Priority_Queue* job_queue = simulator->generated_processes;
    Priority_Queue* terminated_processes = simulator->terminated_processes;
    ProcessPtr front_process;

    /*
     * Represent when process arrives at ready_queue
     * If elapsed time equals arrival time of the process in job queue,
     * pop the process from job queue and insert it in ready queue
     */
    while (!Priority_IsEmpty(job_queue) &&
        (front_process = Priority_Top(job_queue))->arrival_time == simulator->elapsed_time) {
        Priority_Enqueue(ready_queue, front_process);
        Priority_Dequeue(job_queue);
    }



    return;
}

void Simulator_LoadProcess(Simulator* simulator) {
    Queue* ready_queue = simulator->ready_queue;
    Priority_Queue* waiting_queue = simulator->waiting_queue;

    /* Get a process from ready queue and allocate CPU to the process */
    if (simulator->cur_cpu_burst == NULL) {
        if (!Queue_IsEmpty(ready_queue)) {
            simulator->cur_cpu_burst = Queue_Front(ready_queue);
         if (simulator->cur_cpu_burst->isResponsed == 0) simulator->cur_cpu_burst->isResponsed = 1;
            Queue_Dequeue(ready_queue);
        }
    }

    return;
}

void Simulator_LoadProcess_Priority(Simulator* simulator) {
    Priority_Queue* ready_queue = simulator->ready_queue;
    Priority_Queue* waiting_queue = simulator->waiting_queue;

    /* Get a process from ready queue and allocate CPU to the process */
    if (simulator->cur_cpu_burst == NULL) {
        if (!Priority_IsEmpty(ready_queue)) {
            simulator->cur_cpu_burst = Priority_Top(ready_queue);
         if (simulator->cur_cpu_burst->isResponsed == 0) simulator->cur_cpu_burst->isResponsed = 1;
            Priority_Dequeue(ready_queue);
        }
    }

    return;
}

void Simulator_LoadProcess_Priority_Preemptive(Simulator* simulator) {
    Priority_Queue* ready_queue = simulator->ready_queue;
    Priority_Queue* waiting_queue = simulator->waiting_queue;

    /* Get a process from ready queue and allocate CPU to the process */
    if (simulator->cur_cpu_burst == NULL) {
        if (!Priority_IsEmpty(ready_queue)) {
            simulator->cur_cpu_burst = Priority_Top(ready_queue);
        if (simulator->cur_cpu_burst->isResponsed == 0) simulator->cur_cpu_burst->isResponsed = 1;
            Priority_Dequeue(ready_queue);
        }
        simulator->cur_cpu_burst->waiting_time = simulator->cur_cpu_burst->waiting_time - 1;
        simulator->cur_cpu_burst->turnaround_time = simulator->cur_cpu_burst->turnaround_time - 1;
        simulator->cur_cpu_burst->response_time = simulator->cur_cpu_burst->response_time - 1;
    }

    return;
}

void Simulator_Eval(Simulator* simulator) {
    Priority_Queue* terminated_processes = simulator->terminated_processes;
    ProcessPtr tmp;
    double CPU_utilization = (double)(simulator->elapsed_time + 1) / simulator->elapsed_time;

    int pid[100];
    int w_time[100];
    int t_time[100];
    int r_time[100];
    int cnt = 0;

    /* Calculate average waiting time and average turnaround time */
    while (tmp = Priority_Top(terminated_processes)) {
        Priority_Dequeue(terminated_processes);

        simulator->avg_waiting_time += tmp->waiting_time;
        simulator->avg_turnaround_time += tmp->turnaround_time;
        simulator->avg_response_time += tmp->response_time;

        pid[tmp->pid] = tmp->pid;
        w_time[tmp->pid] = tmp->waiting_time;
        t_time[tmp->pid] = tmp->turnaround_time;
        r_time[tmp->pid] = tmp->response_time;

        if (tmp->waiting_time > simulator->max_waiting_time)
            simulator->max_waiting_time = tmp->waiting_time;
        ++cnt;
    }

    simulator->avg_waiting_time /= simulator->num_process;
    simulator->avg_turnaround_time /= simulator->num_process;
    simulator->avg_response_time /= simulator->num_process;

    for (int i = 0; i <cnt; i++) {
        printf("====================================================\n");
        printf("(PID : %d)\n", pid[i]);
        printf("Waiting time : %d\n", w_time[i]);

    }
    printf("====================================================\n");
    printf("Average waiting time : %.3f\n", simulator->avg_waiting_time);

    for (int i = 0; i < cnt; i++) {
        printf("====================================================\n");
        printf("(PID : %d)\n", pid[i]);
        printf("Turnaround time : %d\n", t_time[i]);

    }
    printf("====================================================\n");
    printf("Average turnaround time : %.3f\n", simulator->avg_turnaround_time);

    for (int i = 0; i < cnt; i++) {
        printf("====================================================\n");
        printf("(PID : %d)\n", pid[i]);
        printf("Response time : %d\n", r_time[i]);

    }
    printf("====================================================\n");
    printf("Average response time : %.3f\n", simulator->avg_response_time);
    printf("====================================================\n");

    printf("-> Execution time: %d\n", simulator->elapsed_time + 1);
    printf("-> CPU Utilization: %.3f\n", CPU_utilization);
    printf("-> Average waiting time: %.3f\n", simulator->avg_waiting_time);
    printf("-> Average turnaround time: %.3f\n", simulator->avg_turnaround_time);

    return;
}


int Simulator_CPU_Burst(Simulator* simulator) {
    Priority_Queue* waiting_queue = simulator->waiting_queue;
    Priority_Queue* terminated_processes = simulator->terminated_processes;

    for (int i = 0; i < 1; i++)
        ;

    if (simulator->cur_cpu_burst) { /* running process exists */
        simulator->cur_cpu_burst->burst_time--;
        simulator->cur_cpu_burst->turnaround_time++;
        if (simulator->cur_cpu_burst->isResponsed == 0) simulator->cur_cpu_burst->response_time++;
        printf("[ %6d ]", simulator->cur_cpu_burst->pid); /* display gantt chart */

        if (simulator->cur_cpu_burst->burst_time == 0) { /* Process has completely processed, move to terminated_process */
            simulator->cur_cpu_burst->isResponsed = 1;
            Priority_Enqueue(terminated_processes, simulator->cur_cpu_burst);
            simulator->cur_cpu_burst = NULL;

            /* All processes terminated */
            if (terminated_processes->count == simulator->num_process) {
                printf("\n-> Simulation End.\n\n");
                Simulator_Eval(simulator);

                return 1;
            }
        }
    }
    /* Limit the number of blocks in each line of gantt chart */
    if ((simulator->elapsed_time + 1) % 10 == 0) {
        printf("\n");
    }

    return 0;
}

int Simulator_CPU_Burst_Preemptive(Simulator* simulator) {
    Priority_Queue* ready_queue = simulator->ready_queue;
    Priority_Queue* waiting_queue = simulator->waiting_queue;
    Priority_Queue* terminated_processes = simulator->terminated_processes;
    int (*compare)(ProcessPtr a, ProcessPtr b) = ready_queue->compare;

    for (int i = 0; i < 1; i++)
        ;

    /*
     * Find whether the process
     * by which currently running process can be preempted exists
     */
    if (simulator->cur_cpu_burst && !Priority_IsEmpty(ready_queue) &&
        compare(ready_queue->top->process_ptr, simulator->cur_cpu_burst)) {
        /* Currently running process is preempted */
        simulator->cur_cpu_burst->waiting_time = simulator->cur_cpu_burst->waiting_time + 1;
        simulator->cur_cpu_burst->turnaround_time = simulator->cur_cpu_burst->turnaround_time + 1;
        //simulator->cur_cpu_burst->isResponsed = 1;
        Priority_Enqueue(ready_queue, simulator->cur_cpu_burst);
        simulator->cur_cpu_burst = NULL;
        /* Allocate CPU to another process */
        Simulator_LoadProcess_Priority_Preemptive(simulator);

    }

    if (simulator->cur_cpu_burst) { /* running process exists */
        simulator->cur_cpu_burst->turnaround_time++;
        simulator->cur_cpu_burst->burst_time--;
        simulator->cur_cpu_burst->isResponsed = 1;

       // if (simulator->cur_cpu_burst->isResponsed == 0) simulator->cur_cpu_burst->response_time++;
        printf("[ %6d ]", simulator->cur_cpu_burst->pid); /* display gantt chart */

        if (simulator->cur_cpu_burst->burst_time == 0) { /* Process has completely processed, move to terminated_process */
            Priority_Enqueue(terminated_processes, simulator->cur_cpu_burst);
            simulator->cur_cpu_burst = NULL;

            /* All processes terminated */
            if (terminated_processes->count == simulator->num_process) {
                printf("\n-> Simulation End.\n\n");
                Simulator_Eval(simulator);

                return 1;
            }
        }
    }
    /* Limit the number of blocks in each line of gantt chart */
    if ((simulator->elapsed_time + 1) % 10 == 0) {
        printf("\n");
    }

    return 0;
}

int Simulator_CPU_Burst_RR(Simulator* simulator, int* time_quantum, int maxtime) {
    Queue* ready_queue = simulator->ready_queue;
    Priority_Queue* waiting_queue = simulator->waiting_queue;
    Priority_Queue* terminated_processes = simulator->terminated_processes;

    for (int i = 0; i < 1; i++)
        ;

    if (simulator->cur_cpu_burst) { /* running process exists */
        simulator->cur_cpu_burst->burst_time--;
        simulator->cur_cpu_burst->turnaround_time++;

        //if (simulator->cur_cpu_burst->isResponsed == 0) simulator->cur_cpu_burst->response_time++;
        printf("[ %6d ]", simulator->cur_cpu_burst->pid); /* display gantt chart */
        (*time_quantum)++;

        if (simulator->cur_cpu_burst->burst_time) {
            if (*time_quantum == maxtime) {
                //simulator->cur_cpu_burst->isResponsed = 1;
                Queue_Enqueue(ready_queue, simulator->cur_cpu_burst);
                simulator->cur_cpu_burst = NULL;
                *time_quantum = 0;
            }
        }
        else { /* Process has completely processed, move to terminated_process */
            Priority_Enqueue(terminated_processes, simulator->cur_cpu_burst);
            simulator->cur_cpu_burst = NULL;
            *time_quantum = 0;

            /* All processes terminated */
            if (terminated_processes->count == simulator->num_process) {
                printf("\n-> Simulation End.\n\n");
                Simulator_Eval(simulator);

                return 1;
            }
        }
    }
    /* Limit the number of blocks in each line of gantt chart */
    if ((simulator->elapsed_time + 1) % 10 == 0) {
        printf("\n");
    }

    return 0;
}

void Simulator_Queue_Waiting(Queue* ready_queue) {
    LinkedNodePtr tmp = ready_queue->front;

    if (tmp == NULL) return;

    do {
        tmp->process_ptr->waiting_time++;
        tmp->process_ptr->turnaround_time++;
        if (tmp->process_ptr->isResponsed == 0) tmp->process_ptr->response_time++;
    } while (tmp = tmp->next);

    return;
}

void Simulator_Priority_Waiting(TreeNodePtr waiting_process) {
    if (waiting_process == NULL) return;

    Simulator_Priority_Waiting(waiting_process->left);
    waiting_process->process_ptr->waiting_time++;
    waiting_process->process_ptr->turnaround_time++;
    if (waiting_process->process_ptr->isResponsed == 0) waiting_process->process_ptr->response_time++;
    Simulator_Priority_Waiting(waiting_process->right);

    return;
}

void Simulator_FCFS(Simulator* simulator) {
    Queue* ready_queue = simulator->ready_queue;

    printf("\n# FCFS Algorithm\n\n");
    while (1) {
        Simulator_LoadReadyQueue(simulator);

        Simulator_LoadProcess(simulator);
        Simulator_Queue_Waiting(ready_queue);

        if (Simulator_CPU_Burst(simulator)) return;

        simulator->elapsed_time++;
    }
}

void Simulator_SJF(Simulator* simulator) {
    Priority_Queue* ready_queue = simulator->ready_queue;

    printf("\n# SJF Algorithm\n\n");
    while (1) {
        Simulator_LoadReadyQueue_Priority(simulator);

        Simulator_LoadProcess_Priority(simulator);
        Simulator_Priority_Waiting(ready_queue->top);

        if (Simulator_CPU_Burst(simulator)) return;

        simulator->elapsed_time++;
    }
    return;
}

void Simulator_SRTF(Simulator* simulator) {
    Priority_Queue* ready_queue = simulator->ready_queue;

    printf("\n# SRTF Algorithm\n\n");
    while (1) {
        Simulator_LoadReadyQueue_Priority(simulator);

        Simulator_LoadProcess_Priority(simulator);
        Simulator_Priority_Waiting(ready_queue->top);

        if (Simulator_CPU_Burst_Preemptive(simulator)) return;

        simulator->elapsed_time++;
    }
    return;
}

void Simulator_NonPreemptivePriority(Simulator* simulator) {
    Priority_Queue* ready_queue = simulator->ready_queue;

    printf("\n# Non-Preemptive Priority Algorithm\n\n");
    while (1) {
        Simulator_LoadReadyQueue_Priority(simulator);

        Simulator_LoadProcess_Priority(simulator);
        Simulator_Priority_Waiting(ready_queue->top);

        if (Simulator_CPU_Burst(simulator)) return;

        simulator->elapsed_time++;
    }
    return;
}

void Simulator_RR(Simulator* simulator, int maxtime) {
    Queue* ready_queue = simulator->ready_queue;
    int time_quantum = 0;

    printf("\n# Round Robin Algorithm\n\n");
    while (1) {
        Simulator_LoadReadyQueue(simulator);

        Simulator_LoadProcess(simulator);
        Simulator_Queue_Waiting(ready_queue);

        if (Simulator_CPU_Burst_RR(simulator, &time_quantum, maxtime)) return;

        simulator->elapsed_time++;
    }
    return;
}

void Simulator_Start(Simulator* simulator, int time_quantum) {
    switch (simulator->flag) {
    case FCFS:
        Simulator_FCFS(simulator);
        break;
    case SJF:
        Simulator_SJF(simulator);
        break;
    case SRTF:
        Simulator_SRTF(simulator);
        break;
    case NON_PREEMPTIVE_PRIORITY:
        Simulator_NonPreemptivePriority(simulator);
        break;
    case RR:
        Simulator_RR(simulator, time_quantum);
        break;
    default:
        printf("Error\n");
        exit(-1);
    }

    return;
}

void Simulator_Terminate(Simulator** simulators) {
    printf("\n# Summary\n\n");
    printf(
        "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        "++"
        "+++++++++++++++++\n");
    printf(
        "+++++++++++++++++++++++++++++++  CPU Util  ++   Avg WT   ++   AVG TT  "
        " "
        "++    AVG RT    ++\n");

    for (int i = 0; simulators[i]; i++) {
        switch (simulators[i]->flag) {
        case FCFS:
            printf("++            FCFS           ++  ");
            break;
        case SJF:
            printf("++             SJF           ++  ");
            break;
        case SRTF:
            printf("++            SRTF           ++  ");
            break;
        case RR:
            printf("++             RR            ++  ");
            break;
        case NON_PREEMPTIVE_PRIORITY:
            printf("++   Nonpreemptive Priority  ++  ");
            break;



        default:
            printf("Error\n");
            exit(-1);
        }

        printf(
            "%8.3f  ++ %10.3f ++ %10.3f ++  %10.3f "
            " ++\n",
            (double)(simulators[i]->elapsed_time) /
            simulators[i]->elapsed_time,
            simulators[i]->avg_waiting_time, simulators[i]->avg_turnaround_time,
            simulators[i]->avg_response_time);
    }

    printf(
        "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
        "++"
        "+++++++++++++++++\n");

    return;
}
