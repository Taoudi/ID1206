#include <stdio.h>
#include <pthread.h>
#include "green_atomic.h"
#define THREADS 2
#define MAX 100000
#define PRODUCER 0
#define CONSUMER 1
int flag = PRODUCER;

green_cond_t cond;
green_cond_t cond2;
pthread_cond_t pcond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t pcond2 = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
green_mutex_t mutex;
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
    printf(" count: %d, %d\n",count,id);
    loop--;
    flag = (id+1)%THREADS;
    green_cond_signal(&cond);
  }
  else{
    green_cond_wait(&cond,&mutex);
  }
  }
}

void *test_mutex(void *arg){
  green_mutex_lock(&mutex);
  int id= *(int*)arg;
  while(1){
    printf("we runnin as thread: %d\n", *(int*)arg);
    //loop--;
    if(flag == id%THREADS){
	   //printf("we runnin as thread: %d\n", *(int*)arg);
      flag++;
      green_cond_signal(&cond);
      green_mutex_unlock(&mutex);
    }else {
      //green_cond_signal(&cond);
      green_cond_wait(&cond,&mutex);
    }
  }
}


void *test_consumer(void *arg){
  int id= *(int*)arg;
    while(1){
      green_mutex_lock(&mutex);
  //  pthread_mutex_lock(&lock);
     if(flag%2==CONSUMER){
       printf("Consumed!, %d\n",flag);
      flag++;

      pthread_cond_signal(&pcond1);
     }
      printf("consumer sleep\n" );
      //green_cond_signal(&cond);
      green_cond_wait(&cond2,&mutex);
    //pthread_mutex_unlock(&lock);
  //  pthread_cond_wait(&pcond2,&lock);
    }
}


void *test_producer(void *arg){
  int id = *(int*)arg;
    while(1){
      //pthread_mutex_lock(&lock);
      green_mutex_lock(&mutex);
      if(flag%2==PRODUCER){
      printf("Produced!,%d\n",flag);
      flag++;
      //pthread_cond_signal(&pcond2);
      green_cond_signal(&cond2);
      }
      printf("producer sleep\n" );
      green_cond_wait(&cond,&mutex);
      //pthread_mutex_unlock(&lock);
      //pthread_cond_wait(&pcond1,&lock);
    }
}

int main(){
  green_cond_init(&cond);
  green_cond_init(&cond2);


  green_mutex_init(&mutex);
  green_t g0,g1,g2,g3,g4;
  int a0 = CONSUMER;
  int a1 = PRODUCER;
  int a2 = 2;
  int a3 = 3;
  int a4 = 4;
  pthread_t b0;
  pthread_t b1;
//  pthread_create(&b0,NULL,test_consumer,&a0);
//  pthread_create(&b1,NULL,test_producer,&a1);
flag=PRODUCER;

 green_create(&g1,test_mutex,&a1);
 green_create(&g0,test_mutex,&a0);
//  green_create(&g2,test_mutex,&a2);
//  green_create(&g3,test_mutex,&a3);
//  green_create(&g4,infinity,&a4);

  green_join(&g0);
  green_join(&g1);

//  green_join(&g2);
//  green_join(&g3);
  printf("done, count: %d\n",count);
  return 0;
}
