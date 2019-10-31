#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
long *heap = (unsigned long*) calloc (40, sizeof(unsigned long));

printf("heap[2]: 0x%lx\n", heap[2]);
printf("heap[1]: 0x%lx\n", heap[1]);
printf("heap[0]: 0x%lx\n", heap[0]);
printf("heap[-1]: 0x%lx\n", heap[-1]);
printf("heap[-2]: 0x%lx\n", heap[-2]);
heap[-1]=1000000;

free(heap);
printf("after\n");
printf("heap[-1]: 0x%lx\n", heap[-1]);
printf("heap[-2]: 0x%lx\n", heap[-2]);

	return 0;
}
