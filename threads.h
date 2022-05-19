#include <stdio.h>
#include <stdlib.h>
#include "q.h"
TCB_t* RunQ = NULL;

typedef struct Semaphore {
    struct TCB_t *head;
    int value;
    
} Semaphore;

void InitSem(Semaphore** s, int v)
{
    (*s)->value = v;
    InitQueue(&(*s)->head);
    return;
}

void P(Semaphore** s)
{
	if((*s)->value <= 0){
		struct TCB_t* temp = (struct TCB_t*)malloc(sizeof(struct TCB_t));
		temp = DelQueue(&RunQ);
		AddQueue(&(*s)->head, temp);
		if(RunQ != NULL){
			if((*s)->head->next == NULL){
				swapcontext(&((*s)->head->context), &(RunQ->context));
			}else{
				swapcontext(&((*s)->head->prev->context), &(RunQ->context));
			}
		}else{
			exit(0);
		}
		P(s);
		return;
	}
	(*s)->value--;
    return;
}

void V(Semaphore** s)
{
	(*s)->value++;
	if((*s)->head != NULL){
		struct TCB_t* temp = (struct TCB_t*)malloc(sizeof(struct TCB_t));
		temp = DelQueue(&(*s)->head);
		AddQueue(&RunQ, temp);
	}
    return;
}

void start_thread(void (*function)(int), int id)
{ 
	//allocate a stack (via malloc) of a certain size (choose 8192)
	struct stack *s = malloc(8192);
	//allocate a TCB (via malloc)
	TCB_t* t;
	t = NewItem();
	//call init_TCB with appropriate arguments
	init_TCB (t, function, s, 8192, id);
	//call addQ to add this TCB into the "RunQ" which is a global header pointer
	AddQueue(&RunQ, t);
	return;
}

void run()
{	// real code
	ucontext_t parent;	// get a place to store the main context, for faking
	getcontext(&parent);	// magic sauce
	swapcontext(&parent, &(RunQ->context));	// start the first thread
	return;
}

void yield() // similar to run
{
	//rotate the run Q
	RotateQ(&RunQ);
	//swap the context, from previous thread to the thread pointed to by RunQ
	if(RunQ->next != NULL && RunQ != NULL){
		swapcontext(&(RunQ->prev->context), &(RunQ->context));
	}
	return;
}
