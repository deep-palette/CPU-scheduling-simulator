#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "DataStructure.h"
#include "ProcessSimulator.h"


int main()
{
	int ProcessCnt, time_quantum;
	Simulator* simulator[6];

	initSimulator(&simulator[0], FCFS);
	initSimulator(&simulator[1], SJF);
	initSimulator(&simulator[2], SRTF);
	initSimulator(&simulator[3], RR);
	initSimulator(&simulator[4], NON_PREEMPTIVE_PRIORITY);
	simulator[5] = NULL;

	//printf("프로세스 개수를 입력해주세요 : ");
	//scanf("%d", &ProcessCnt);

	Simulator_GenerateProcess(simulator, &time_quantum);

	//printf("시간할당량을 입력하세요 : ");
	//scanf("%d", &time_quantum);

	for (int i = 0; i < 5; i++) Simulator_Start(simulator[i], time_quantum);

	Simulator_Terminate(simulator);

	system("pause");
	return 0;


	


}