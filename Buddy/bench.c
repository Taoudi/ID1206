#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "buddy.h"

#define ROUNDS 8
#define LOOPS 100000
#define BUFFER 20000
#define MIN 1
#define PAGE_SIZE 4072



void randinit(){
  time_t t;
  srand(t);
}
int request() {
  /* k is log(MAX/MIN) */
  double k = log(((double) PAGE_SIZE )/ MIN);
  /* r is [0..k[ */
  double r = ((double)( rand() % (int)(k*10000))) / 10000;
  /* size is [0 .. MAX[ */
  int size = (int)((double)PAGE_SIZE / exp(r));
  //printf("SIZE: %d \n", size);
  return size;
}
/**
int request(){
  int size = rand()%(PAGE_SIZE);
  return size;
}
**/



void printB(int* address){
printf("ADDRESS: %p \n",address);
}

void speed(){
  void *buffer[BUFFER];
  double bfrees[LOOPS];
  double ballocs[LOOPS];
  int sizes[LOOPS];
  double avgtimesb[ROUNDS];
  double avgtimesf[ROUNDS];
   for(int i =0; i < BUFFER; i++) {
     buffer[i] = NULL;
   }
 for(int i =0; i < LOOPS; i++) {
     ballocs[i] = 0;
   }
	for(int i =0; i < LOOPS; i++) {
     sizes[i] = 0;
   }

 clock_t start = clock();
 double timesb[LOOPS];
 double timesf[LOOPS];
   for(int j = 0; j < ROUNDS; j++) {
     double balloctime = 0;
	double freetime = 0;
 	for(int i =0; i < BUFFER; i++) {
     		buffer[i] = NULL;
  	 }
     for(int i= 0; i < LOOPS ; i++) {
       int index = rand() % BUFFER;
       clock_t start1 = clock();
       double elapsedfree = 0;
       if(buffer[index] != NULL) {
         clock_t startfree = clock();
 	       free(buffer[index]);
          elapsedfree = (double)(clock() - startfree)*1000/ CLOCKS_PER_SEC;
	freetime+=elapsedfree;
       }
       size_t size = pow(2,(j+5))-25;
	sizes[i]=level(size);
       int *memory;
        clock_t startballoc = clock();
		      memory = malloc(size);
        double elapsedballoc = (double)(clock() - startballoc)*1000 / CLOCKS_PER_SEC;
        balloctime +=elapsedballoc;
       if(memory == NULL) {
 	       memory = malloc(0);
 	       return;
       }
   double elapsed = (double)(clock() - start)*1000 / CLOCKS_PER_SEC;
    //   printf("%d\t%f\t%f\n", i,elapsedballoc,elapsedfree);
       buffer[index] = memory;
       /* writing to the memory so we know it exists */
      *memory = 123;
     }

     avgtimesb[j]=balloctime;
     printf("%d\t%f\t%f\n",j,balloctime,freetime);

     // Execuatable code

/**
     int count = 0;
     for(int i =0; i<BUFFER;i++){
       if(buffer[i]!=NULL){
       count++;
      }
     }
  printf("Elements in buffer: %d----------",count);
     **/
 }
}





void memory(){

  void *buffer[BUFFER];
  for(int i =0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }
 double  start = clock();
  for(int j = 0; j < ROUNDS; j++) {
    for(int i= 0; i < LOOPS; i++) {
      int index = rand() % BUFFER;
      if(buffer[index] != NULL) {
	free(buffer[index]);
      }
      size_t size = (size_t)request();
      int *memory;
      memory = malloc(size);

      if(memory == NULL) {
	memory = balloc(0);
	fprintf(stderr, "memory myllocation failed, last address %p\n", memory);
  return;
      }

      buffer[index] = memory;
      /* writing to the memory so we know it exists */
      // *memory = 123;
            printf("%d\t%d\t%d\t%d \n", (int)(((double) (clock() - start)*1000*1000) / CLOCKS_PER_SEC),getTotal(), externalFragmentation(), getTotal()-externalFragmentation());
    }
  }
}

int main(){
  randinit();
  speed();
  //memory();
}
