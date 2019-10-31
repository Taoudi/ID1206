#include <stdio.h>
#include <pthread.h>
#include "green_atomic.h"
#define THREADS 4
#define MAX 100000
#define PRODUCER 0
#define CONSUMER 1
int flag = PRODUCER;


green_cond_t cond;
green_cond_t cond2;

green_mutex_t mutex;

pthread_cond_t pcond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t pcond2 = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int count = 0;


void *test(void *arg){
  int i = *(int*)arg;
  int loop = 4;
  while(loop>0){
    loop--;
    green_yield();
  }
}

void *infinity(void *arg){
  while(count<MAX){
    printf("im stuck\n");
  }
}

void *test_timer(void *arg){
  int i = *(int*)arg;
  int loop = MAX;
  while(loop>0){
    green_mutex_lock(&mutex);
    count++;
    printf(" count: %d, thread: %d\n",count, i);
	loop--;
    green_mutex_unlock(&mutex);

  }
}

void *test_cond(void *arg){
  int id= *(int*)arg;
  int loop = MAX;
  while(loop>0){
    if(flag==id){
    count++;
    printf(" count: %d\n",count);
    loop--;
    flag = (id+1)%THREADS;
    green_cond_signal(&cond);
  }
  else{
   //green_cond_wait(&cond);
  }
  }
}

void *test_mutex(void *arg){
  green_mutex_lock(&mutex);
while(1){
	printf("we runnin as thread: %d\n", *(int*)arg);
  if(flag == 0){
    flag=1;
    green_cond_signal(&cond);
    green_mutex_unlock(&mutex);
    break;
  }else {
    // green_cond_signal(&cond);
    green_cond_wait(&cond,&mutex);
  }

}
}


void *test_consumer(void *arg){
  int id= *(int*)arg;
    while(1){
      green_mutex_lock(&mutex);
     if(flag%2==CONSUMER){
       printf("Consumed!, %d\n",flag);
      flag++;
      green_cond_signal(&cond);
     }
   //   printf("consumer sleep\n" );
    green_cond_wait(&cond2,&mutex);
    }
}
void *test_producer(void *arg){
  int id = *(int*)arg;
    while(1){
      green_mutex_lock(&mutex);
      if(flag%2==PRODUCER){
      printf("Produced!,%d\n",flag);
      flag++;
      green_cond_signal(&cond2);
      }
     // printf("producer sleep\n" );
      green_cond_wait(&cond,&mutex);
    }
}


void *pt_consumer(void *arg){
  int id= *(int*)arg;
    while(1){
      //green_mutex_lock(&mutex);
      pthread_mutex_lock(&lock);
     if(flag%2==CONSUMER){
       printf("Consumed!, %d\n",flag);
      flag++;
    //  green_cond_signal(&cond);

      pthread_cond_signal(&pcond1);
     }
   //   printf("consumer sleep\n" );
    //green_cond_wait(&cond2,&mutex);
    pthread_mutex_unlock(&lock);
      pthread_cond_wait(&pcond2,&lock);
    }
}
void *pt_producer(void *arg){
  int id = *(int*)arg;
    while(1){
      pthread_mutex_lock(&lock);
      //green_mutex_lock(&mutex);
      if(flag%2==PRODUCER){
      printf("Produced!,%d\n",flag);
      flag++;
      pthread_cond_signal(&pcond2);
      //green_cond_signal(&cond2);
      }
     // printf("producer sleep\n" );
      //green_cond_wait(&cond,&mutex);
      pthread_mutex_unlock(&lock);
      pthread_cond_wait(&pcond1,&lock);
    }
}


}
