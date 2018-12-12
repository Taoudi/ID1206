#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include <stdio.h>
#include "green_atomic.h"
#include <signal.h>
#include <sys/time.h>
#define PERIOD 100
#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096



static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx,NULL,NULL,NULL,NULL,FALSE};

static green_t *running = &main_green;
static green_t *queue = NULL;
static sigset_t block;

void timer_handler(int);


static void init() __attribute__((constructor));


void init(){
	getcontext(&main_cntx);
	sigemptyset(&block);
	sigaddset(&block,SIGVTALRM);

	struct sigaction act = {0};
	struct timeval interval;
	struct itimerval period;

	act.sa_handler = timer_handler;
	assert(sigaction(SIGVTALRM,&act,NULL)==0);

	interval.tv_sec=0;
	interval.tv_usec = PERIOD;
	period.it_interval=interval;
	period.it_value = interval;
	setitimer(ITIMER_VIRTUAL,&period,NULL);
}


void printlist(green_t *list){
	while(list!=NULL){
		printf("block: %p->", list);
		list=list->next;
	}
	printf("NULL;");
	printf("\n");
}
void enqueue(green_t *node){
	if(queue==NULL){
		queue = node;
	}
	else if(queue->next == NULL){
		queue->next = node;
	}
	else{
			green_t *temp = queue;
			while(temp->next != NULL){
				temp = temp->next;
			}
			temp->next = node;
	}
}

void join_enqueue(green_t *node,green_t *list){
	if(list->join != NULL){
		green_t *current = list->join;
		while(current->next != NULL){
			current = current->next;
		}
		current->next=node;
	}
	else{
			list->join = node;
	}
}

green_t* dequeue(){
	green_t* next = queue;
	queue = next->next;
	next->next = NULL;
	return next;
}

void susp_enqueue(green_t *node,green_mutex_t *list){
	if(list->suspend == NULL){
		list->suspend = node;
	}
	else{
		green_t *current = list->suspend;
		while(current->next != NULL){
			current = current->next;
		}
		current->next=node;
	}
}


green_t* susp_dequeue(green_mutex_t* list){
	if(list->suspend != NULL){
		green_t* next = list->suspend;
	  list->suspend = NULL;
		return next;
	}
	return NULL;
}

int green_mutex_init(green_mutex_t *mutex){
	mutex->taken = FALSE;
	mutex->suspend=NULL;
}

int green_mutex_lock(green_mutex_t *mutex){
	sigprocmask(SIG_BLOCK,&block,NULL);

	green_t *susp = running;
	while(mutex->taken){
		susp_enqueue(susp,mutex);
		green_t *next = dequeue();
		running = next;
		swapcontext(susp->context,next->context);
	}
	mutex->taken = TRUE;
	sigprocmask(SIG_UNBLOCK,&block,NULL);
}

int green_mutex_unlock(green_mutex_t *mutex){
	sigprocmask(SIG_BLOCK,&block,NULL);
	enqueue(susp_dequeue(mutex));
	mutex->taken = FALSE;
	sigprocmask(SIG_UNBLOCK,&block,NULL);
}

void green_thread(){
	//printf("test\n");
	sigprocmask(SIG_UNBLOCK,&block,NULL);
	green_t *this = running;
	(*this->fun)(this->arg);

	sigprocmask(SIG_BLOCK,&block,NULL);
	enqueue(this->join);
	free(this->context);
	this->zombie = TRUE;
  green_t *next = dequeue();
	running = next;
	setcontext(next->context);
	sigprocmask(SIG_UNBLOCK,&block,NULL);

}

int green_create(green_t *new, void *(*fun)(void*), void*arg){
	ucontext_t *cntx = (ucontext_t *)malloc(sizeof(ucontext_t));
	getcontext(cntx);
	void *stack = malloc(STACK_SIZE);
	cntx->uc_stack.ss_sp = stack;
	cntx->uc_stack.ss_size = STACK_SIZE;

	makecontext(cntx,green_thread,0);
	new->context = cntx;
	new->fun = fun;
	new->arg = arg;
	new->next = NULL;
	new->join = NULL;
	new->zombie = FALSE;
	sigprocmask(SIG_BLOCK,&block,NULL);
	enqueue(new);
	sigprocmask(SIG_UNBLOCK,&block,NULL);
	return 0 ;
}

int green_yield(){
	sigprocmask(SIG_BLOCK,&block,NULL);
	green_t *susp = running;
	enqueue(susp);
	green_t *next = dequeue();
	running = next;
	swapcontext(susp->context, next->context);
	sigprocmask(SIG_UNBLOCK,&block,NULL);

	return 0;
}


int green_join(green_t *thread){
		if(thread->zombie)
		return 0;
	green_t *susp = running;
	 sigprocmask(SIG_BLOCK, &block, NULL);

	join_enqueue(susp,thread);
	green_t *next = dequeue();
	running = next;
	swapcontext(susp->context,next->context);
	sigprocmask(SIG_UNBLOCK,&block,NULL);


	return 0;
}

void cond_enqueue(green_t *node,green_cond_t *list){
	if(list->first == NULL){
		list->first = node;
	}
	else{
		green_t *current = list->first;
		while(current->next != NULL){
			current = current->next;
		}
		current->next=node;
	}
}

green_t* cond_dequeue(green_cond_t* list){
	if(list->first != NULL){
		green_t* next = list->first;
		list->first = next->next;
		next->next = NULL;
		return next;
	}
	return NULL;
}

void timer_handler(int sig){
	green_t *susp = running;
	enqueue(running);
	green_t *next = dequeue();
	running = next;
	swapcontext(susp->context,next->context);
}

void green_cond_init(green_cond_t* cond){
	cond->first = NULL;
}

void green_cond_wait(green_cond_t* cond, green_mutex_t *mutex){
	sigprocmask(SIG_BLOCK,&block,NULL);
	green_t *susp = running;
	cond_enqueue(susp,cond);
	if(mutex!=NULL){
		mutex->taken = FALSE;
		enqueue(susp_dequeue(mutex));
	}
	green_t *next = dequeue();
	running = next;
	swapcontext(susp->context,next->context);

	if(mutex != NULL){
		while(mutex->taken){
			susp_enqueue(susp,mutex);
			green_t *next = dequeue();
			running = next;
			swapcontext(susp->context,next->context);
		}
		mutex->taken = TRUE;
	}
	sigprocmask(SIG_UNBLOCK,&block,NULL);
}

void green_cond_signal(green_cond_t* cond){
	sigprocmask(SIG_BLOCK,&block,NULL);
	enqueue(cond_dequeue(cond));
	sigprocmask(SIG_UNBLOCK,&block,NULL);

}
