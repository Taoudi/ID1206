#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
	
		char *heap = malloc(20);
		*heap = 0x61;
		printf("pointing to: 0x%x\n", *heap);
		free(heap);

		char *foo = malloc(20);
		*foo = 0x62;
		printf("foo pointing to: 0x%x \n",*foo);


		/* danger ahead*/
		//char command[50];
		*heap = 0x63;
		printf("or is it pointing to: 0x%x\n",*foo);

	return 0;

}

