#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "rand.h"

#define ROUNDS 10
#define LOOP 100000
#define BUFFER 100

int main(){
  void *buffer[BUFFER];
  	for(int i = 0; i < BUFFER; i++) {
  		buffer[i] = NULL;
  	}

  void *init = sbrk(0);	// get current top of the heap
  void *current;
printf("The initial top of the heap is %p.\n", init);

for(int j = 0; j < ROUNDS; j++) {
		for(int i = 0; i < LOOP; i++) {

			int index = rand() % BUFFER;
			if(buffer[index] != NULL) {
				free(buffer[index]);
			}

			size_t size = (size_t) request();
			int *memory;
			memory = malloc(size);

			if(memory == NULL) {
				fprintf(stderr, "memory allocation failed\n");
				return(1);
			}

			buffer[index] = memory;
			/* writing to the memory so we know it exits */
			*memory = 123;
}
  current =sbrk(0);
  int allocated = (int)((current-init)/1024);
  printf("%d\n",j);
  printf("The current top of the heap %p.\n",current);
  printf("  Increased by %d Kbyte\n", allocated);
}
return 0;
}
