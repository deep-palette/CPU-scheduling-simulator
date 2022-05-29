#include <stdio.h>
#include <stdlib.h> 
#include "DataStructure.h"


int Queue_Init(Queue** queue)
{
    *queue = (Queue*)malloc(sizeof(Queue));

    if ((*queue) == NULL) {
        printf("Error log: Queue_Init error\n");
        exit(-1);
    }

    (*queue)->count = 0;
    (*queue)->front = (*queue)->rear = NULL;

    return 1;
}


int Queue_Enqueue(Queue* queue, ProcessPtr process)
{
    LinkedNodePtr new = (LinkedNodePtr)malloc(sizeof(LinkedNode)); //node�����ؼ� process�ֱ�

    if (new == NULL) {
        printf("Error log: Queue_Enqueue error\n");
        exit(-1);
    }

    new->process_ptr = process; //node�� process�ֱ�
    new->next = NULL;

    if (queue->count == 0) {  
        /* The inserted node is the only node in queue. */
        queue->front = queue->rear = new;
    }
    else {
        queue->rear->next = new; //������ node��next�� new �߰�
        queue->rear = new; // queue�ڿ��� ���̱�
    }

    queue->count++;
    return 1;
}


int Queue_Dequeue(Queue* queue)
{
    if (queue->count == 0)
    {
        return 0;
    }
    else {
        LinkedNodePtr tmp = queue->front;
        queue->front = tmp->next;
        free(tmp);

        /* There is no node in queue after deletion. */
        if (--queue->count == 0) queue->rear = NULL;

        return 1;
    }
    
}


ProcessPtr Queue_Front(Queue* queue)
{
    if (queue->count == 0) return NULL;

    return queue->front->process_ptr;
}


int Queue_IsEmpty(Queue* queue)
{
    return queue->count == 0; //empty�� 1 ��ȯ
}

