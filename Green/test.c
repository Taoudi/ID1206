#include <stdio.h>
#include "green.h"
#define THREADS 4
#define MAX 1000000
int flag = 0;
green_cond_t cond;
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
    printf(" count: %d,%d\n",count,id);
    loop--;
    flag = (id+1)%THREADS;
  }
  else{
    green_cond_signal(&cond);

    green_cond_wait(&cond);
  }
  }
}

void *test_mutex(void *arg){
  green_mutex_lock(&mutex);
  int id= *(int*)arg;
  while(1){
    printf("runnin as thread: %d\n",*(int*)arg);
    if(flag == id){
    flag=1;
    green_cond_signal(&cond);
    green_mutex_unlock(&mutex);
    break;
  }else {
    green_mutex_unlock(&mutex);
    green_cond_wait(&cond);
  }
}
}

int main(){
  green_cond_init(&cond);
  green_mutex_init(&mutex);
  green_t g0,g1,g2,g3,g4;
  int a0 = 0;
  int a1 = 1;
  int a2 = 2;
  int a3 = 3;
  int a4 = 4;
  green_create(&g0,test_mutex,&a0);
  green_create(&g1,test_mutex,&a1);
  green_create(&g2,test_mutex,&a2);
  green_create(&g3,test_mutex,&a3);
//  green_create(&g4,infinity,&a4);

  green_join(&g0);
  green_join(&g1);
  green_join(&g2);
  green_join(&g3);
  printf("done, count: %d\n",count);
  return 0;
}
