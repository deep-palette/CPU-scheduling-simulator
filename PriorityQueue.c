#include <stdio.h>
#include <stdlib.h>
#include "DataStructure.h"
#include "ProcessSimulator.h"

#define SWAP(a, b)                   \
  ProcessPtr tmp = a->process_ptr;   \
  a->process_ptr = b->process_ptr;   \
  b->process_ptr = tmp;
#define parent(x) x->parent


int Priority_Init(Priority_Queue** queue, int (*compare)(ProcessPtr a, ProcessPtr b)) //���� Priority Queue����
{
    *queue = (Priority_Queue*)malloc(sizeof(Priority_Queue));

    if ((*queue) == NULL) {
        printf("Error log: Priority_Init error\n");
        exit(-1);
    }

    (*queue)->count = 0;
    (*queue)->top = NULL;
    (*queue)->compare = compare;

    return 1;
}



int Priority_Enqueue(Priority_Queue* queue, ProcessPtr process)  //�ִ� Priority Queue�� ���μ��� �߰�
{
    int pos; /* varaible used for finding the place at which insert the node */
    TreeNodePtr walker;


    TreeNodePtr new = (TreeNodePtr)malloc(sizeof(TreeNode));
    if (new == NULL) {
        printf("Error log: Priority_Enqueue error\n");
        exit(-1);
    }

    new->process_ptr = process; //���ȿ� ���μ����� ����
    new->left = new->right = new->parent = NULL;

    /* The inserted node is the first node in queue. */
    if (queue->count++ == 0) {
        queue->top = new;
        return 1;
    }


    //���� �����ϴ� ���� queue->count�� �ּ� 2������
    //complete binary tree�ε�
    /* Find the place at which insert the new node */
    walker = queue->top;
    for (pos = 2; !(pos <= queue->count && queue->count < (pos * 2 )); pos = pos *2);
    
    pos = pos / 2 ;
    //������
    //pos�� hight -1���ε�
    //�� pos = queue->count/2 - 1�ϸ� �Ǵ°žƴѰ�?




    while (pos >= 2) {
        if (pos & queue->count)
            walker = walker->right;
        else
            walker = walker->left;

        pos = pos >> 1;
    }
    //pos�� �׻� 1�� �����°� �ƴ�?

    //���ؾȵ�

    /* Insert the new node in heap */
    if (pos & queue->count) { 
        walker->right = new;
        new->parent = walker;
    }
    else {
        walker->left = new;
        new->parent = walker;
    }


    /* Heap Adjustments */
    walker = new;
    while (walker->parent != NULL) {
        if (queue->compare(walker->process_ptr, parent(walker)->process_ptr))
        {
            SWAP(walker, parent(walker));
            walker = walker->parent;
        }
        else {
            break;
        }
    }

    return 1;
}



int Priority_Dequeue(Priority_Queue* queue)
{
    int pos; /* varaible used for finding the place from which delete the node */
    TreeNodePtr walker;

    if (queue->count == 0) return 0;

    if (queue->count == 1) {
        free(queue->top);
        queue->count--;
        queue->top = NULL;

        return 1;
    }

    /* Find the last node in heap */
    walker = queue->top;
    for (pos = 2; !(pos <= queue->count && queue->count < (pos << 1));
        pos = pos << 1)
        ;
    pos = pos >> 1;

    while (pos >= 2) {
        if (queue->count & pos)
            walker = walker->right;
        else
            walker = walker->left;

        pos = pos >> 1;
    }

    /* Delete the last node */
    if (pos & queue->count) {
        queue->top->process_ptr = walker->right->process_ptr;
        free(walker->right);
        walker->right = NULL;
    }
    else {
        queue->top->process_ptr = walker->left->process_ptr;
        free(walker->left);
        walker->left = NULL;
    }

    /* Heap Adjustments */
    queue->count--;
    walker = queue->top;
    while (1) {
        /* Walker node is leaf node */
        if (walker->left == NULL && walker->right == NULL) break;

        TreeNodePtr child = walker->right
            ? queue->compare(walker->left->process_ptr,
                walker->right->process_ptr)
            ? walker->left
            : walker->right
            : walker->left;
        if (queue->compare(child->process_ptr, walker->process_ptr)) {
            SWAP(child, walker);
            walker = child;
        }
        else {
            break;
        }
    }

    return 1;
}



ProcessPtr Priority_Top(Priority_Queue* queue)
{
    if (queue->count == 0) return NULL;

    return queue->top->process_ptr;
}



int Priority_IsEmpty(Priority_Queue* queue)
{
    return queue->count == 0; //empty�̸� 1���
}


int SJF_Compare(ProcessPtr a, ProcessPtr b)
{
    return a->burst_time != b->burst_time
        ? a->burst_time < b->burst_time
        : a->pid < b->pid;
} //bursttime ������ pid�� ���� / bursttime �ٸ��� bursttime �� ������ - a�� ������ 1��� b�� ������ 0���


int Priority_Compare(ProcessPtr a, ProcessPtr b)
{
    return a->priority != b->priority ? a->priority < b->priority
        : a->pid < b->pid;
}   //priority���� ������ Ƣ���