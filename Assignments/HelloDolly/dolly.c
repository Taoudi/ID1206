#include <stdio.h>
#include <unistd.h>

int main(){
	int x=123;
	int pid=fork();
	
	if(pid == 0){
	printf("child: x is  %d\n", x);
	x=42;
	sleep(1);
	printf(" child: x is %d\n",x);
	}
	else{
	printf(" mother: x is %d\n", x);
	x=13;
	sleep(1);
	printf(" mother: x is %d\n",x);
	wait(NULL);
	}	
	return 0;
}
