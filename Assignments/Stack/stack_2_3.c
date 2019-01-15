#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void zot(unsigned long *stop){
	unsigned long r = 0x3;
	unsigned long *i;
	for(i=&r;i<=stop;i++){
		printf("%p    0x%lx\n"  , i,*i);
	}
}



void foo(unsigned long *stop,int one, int two, int three, int four,int five
		,int six,int seven, int eight, int nine){
	unsigned long q = 0x2;
	zot(stop);
}

int main(){

	int pid=getpid();
	printf("%d\n", pid);
	unsigned long p=0x1;
	foo(&p,1,2,3,4,5,6,7,8,9);

	back:
		printf(" p: %p \n", &p);
		printf(" back: %p \n", &&back);
		printf("\n\n /proc/%d/maps \n\n", pid);
		char command[50];
		sprintf(command, "cat /proc/%d/maps", pid);
		system(command);

	return 0;

}
