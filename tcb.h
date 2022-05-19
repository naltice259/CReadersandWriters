#include <ucontext.h>

typedef struct TCB_t {
	struct TCB_t	*next;
	struct TCB_t	*prev;
	ucontext_t	context;
} TCB_t;	



void init_TCB (TCB_t *tcb, void (*function)(int), void *stackP, int stack_size, int id) {
	memset(tcb, '\0', sizeof(TCB_t)); 
	getcontext(&tcb->context);
	// wash, rinse
	// have to get parent context, else snow forms on hell 
	tcb->context.uc_stack.ss_sp = stackP;
	tcb->context.uc_stack.ss_size = (size_t) stack_size; 
	makecontext(&tcb->context, function, 1, id);
	// context is now cooked
	return;
}

