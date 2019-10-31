#include <stdio.h>
#include <unistd.h>

int main(){
//	int x=123;
	int pid=fork();
	
if(pid == 0){
	printf("child is  %d\n", pid);
	sleep(1);
}
else{
	//printf(" my child is called: x is %d\n", pid);
	int res;
	wait(&res);
	printf("result was: ", WEXITSATUS(res));
}
//	printf("done %d\n", pid);
	return 0;
}
