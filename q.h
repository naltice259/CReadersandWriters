#include <stdio.h>
#include <stdlib.h>
#include "tcb.h"
/*
typedef struct Queue {
    struct Queue* next;
    struct Queue* prev;
    int payload;
    
} Queue;
*/
void InitQueue(TCB_t** q)
{
    *q = NULL;
    return;
}

void AddQueue(TCB_t** q, TCB_t* item)
{
	if (*q == NULL){
		*q = item;
	}else if ((*q)->next == NULL || (*q)->next == *q){
		item->prev = *q;
		item->next = *q;
		(*q)->next = item;
 		(*q)->prev = item;
 		
	}else{
		item->prev = (*q)->prev;
		item->next = *q;
		(*q)->prev->next = item;
 		(*q)->prev = item;
	}	
    return;
}

struct TCB_t* DelQueue(TCB_t** q)
{
    // If queue is empty, return NULL.
    if (*q == NULL){
    	return NULL;
	}else if ((*q)->next == NULL || (*q)->next == *q){
		struct TCB_t* temp = (struct TCB_t*)malloc(sizeof(struct TCB_t));
		temp->next = NULL;
    	temp->prev = NULL;
    	temp->context = (*q)->context;
    	*q = NULL;
    	return temp;
	}else{
		struct TCB_t* temp = (struct TCB_t*)malloc(sizeof(struct TCB_t));
		temp->next = NULL;
    	temp->prev = NULL;
    	temp->context = (*q)->context;
    	(*q)->prev->next = (*q)->next;
 		(*q)->next->prev = (*q)->prev;
    	*q = (*q)->next;
 		return temp;
	}
}

void RotateQ(TCB_t** q)
{
    if (*q != NULL && (*q)->next != NULL){
    	*q = (*q)->next;
	}
    return;
}

struct TCB_t* NewItem()
{
    struct TCB_t* q = (struct TCB_t*)malloc(sizeof(struct TCB_t));
    q->next = NULL;
    q->prev = NULL;
    return q;
}
