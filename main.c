#include <stdio.h>
#include "DataStructure.h"


int main()
{
	int N;
	

	int Priority_Init(Priority_Queue * *queue,
	int (*compare)(ProcessPtr a, ProcessPtr b));
	int Priority_Enqueue(Priority_Queue * queue, ProcessPtr process);
	int Priority_Dequeue(Priority_Queue * queue);
	ProcessPtr Priority_Top(Priority_Queue * queue);
	int Priority_IsEmpty(Priority_Queue * queue);
	int SJF_Compare(ProcessPtr a, ProcessPtr b);
	int Priority_Compare(ProcessPtr a, ProcessPtr b);
	int IO_Burst_Compare(ProcessPtr a, ProcessPtr b);
	


}