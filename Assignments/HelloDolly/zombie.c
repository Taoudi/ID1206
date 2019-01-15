

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
int pid = fork();
if(pid==0){
	printf("check the status\n");
	sleep(2);
	printf("and again\n");
	sleep(5);
	return 42;
}else{
	sleep(1);
	int res;
	printf("paretn waiting\n");
	wait(&res);
	printf("The result was %d\n", WEXITSTATUS(res));
	printf("and again\n");
	sleep(1);
}
return 0;
}
