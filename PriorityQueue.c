#include <stdio.h>
#include <stdlib.h>
#include "DataStructure.h"
#include "ProcessSimulator.h"

#define SWAP(a, b)                   \
  ProcessPtr tmp = a->process_ptr;   \
  a->process_ptr = b->process_ptr;   \
  b->process_ptr = tmp;
#define parent(x) x->parent


int Priority_Init(Priority_Queue** queue, int (*compare)(ProcessPtr a, ProcessPtr b)) //새로 Priority Queue생성
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



int Priority_Enqueue(Priority_Queue* queue, ProcessPtr process)  //있는 Priority Queue에 프로세스 추가
{
    int pos; /* varaible used for finding the place at which insert the node */
    TreeNodePtr walker;


    TreeNodePtr new = (TreeNodePtr)malloc(sizeof(TreeNode));
    if (new == NULL) {
        printf("Error log: Priority_Enqueue error\n");
        exit(-1);
    }

    new->process_ptr = process; //노드안에 프로세스를 넣음
    new->left = new->right = new->parent = NULL;

    /* The inserted node is the first node in queue. */
    if (queue->count++ == 0) {
        queue->top = new;
        return 1;
    }


    //여기 진입하는 경우는 queue->count가 최소 2부터임
    //complete binary tree인듯
    /* Find the place at which insert the new node */
    walker = queue->top;
    for (pos = 2; !(pos <= queue->count && queue->count < (pos * 2 )); pos = pos *2);
    
    pos = pos / 2 ;
    //제안점
    //pos는 hight -1값인듯
    //걍 pos = queue->count/2 - 1하면 되는거아닌가?




    while (pos >= 2) {
        if (pos & queue->count)
            walker = walker->right;
        else
            walker = walker->left;

        pos = pos >> 1;
    }
    //pos는 항상 1로 나오는거 아님?

    //이해안됨

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
    return queue->count == 0; //empty이면 1출력
}


int SJF_Compare(ProcessPtr a, ProcessPtr b)
{
    return a->burst_time != b->burst_time
        ? a->burst_time < b->burst_time
        : a->pid < b->pid;
} //bursttime 같으면 pid값 비교함 / bursttime 다르면 bursttime 중 작은거 - a가 작으면 1출력 b가 작으면 0출력


int Priority_Compare(ProcessPtr a, ProcessPtr b)
{
    return a->priority != b->priority ? a->priority < b->priority
        : a->pid < b->pid;
}   //priority값이 작은게 튀어나옴