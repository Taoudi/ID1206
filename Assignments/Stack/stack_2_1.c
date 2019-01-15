#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

char global[] = "This is a global string";
const int read_only = 12345;

int foo(){
	int i = 3;
	return &i;
}


int main() {
    
    int pid = getpid();
    int *z = foo(); 
	printf("%d\n", z);
 //  unsigned long p = 0x1;

 //  printf("p (0x%lx): %p \n", p, &p);
 //  printf("\n\n /proc/%d/maps", pid);
 //	char command[50];
 //	sprintf(command, "cat /proc/%d/maps", pid);
 //	system(command);

	
	
 
        return 0;
}
