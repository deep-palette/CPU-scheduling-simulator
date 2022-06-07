#ifndef __DataStructure_
#define __DataStructure_



typedef struct _Process {
	int pid;          /* unique identifier of a process */
	int burst_time; /* time length to be processed by CPU */
//	int io_burst_time;  /* time length to be processed by I/O */ //필요한가??
	int arrival_time;   /* time at which process has created */
	int priority;              /* the smaller value is, the higher priority is */
	int isResponsed;

	int waiting_time;    /* waiting time of process */
	int turnaround_time; /* turnaround time of process */
	int response_time;
} Process, * ProcessPtr;



typedef struct _LinkedNode {
	ProcessPtr process_ptr; /* pointer of a process */
	struct _LinkedNode * next;    /* pointer used for singly linked list */
} LinkedNode, * LinkedNodePtr;


typedef struct _Queue {
	int count;
	LinkedNodePtr front, rear;
} Queue;

int Queue_Init(Queue** queue);
int Queue_Enqueue(Queue* queue, ProcessPtr process);
int Queue_Dequeue(Queue* queue);
ProcessPtr Queue_Front(Queue* queue);
int Queue_IsEmpty(Queue* queue);




/* Priority Queue Implementation */
typedef struct _TreeNode {
	ProcessPtr process_ptr;               /* pointer of a process */
	struct _TreeNode* parent,* left,* right; /* pointer for binary tree */
} TreeNode, * TreeNodePtr;

typedef struct _Priority_Queue {
	int count;
	TreeNodePtr top;
	int (*compare)(ProcessPtr a, ProcessPtr b); /* compare function pointer */
} Priority_Queue;

int Priority_Init(Priority_Queue** queue,
	int (*compare)(ProcessPtr a, ProcessPtr b));
int Priority_Enqueue(Priority_Queue* queue, ProcessPtr process);
int Priority_Dequeue(Priority_Queue* queue);
ProcessPtr Priority_Top(Priority_Queue* queue);
int Priority_IsEmpty(Priority_Queue* queue);
int SJF_Compare(ProcessPtr a, ProcessPtr b);
int Priority_Compare(ProcessPtr a, ProcessPtr b);

#endif