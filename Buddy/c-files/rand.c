#include <math.h>

#define MAX 4000
#define MIN 1

int request(){
/* k is log(MAX/MIN)*/
double k = log(((double) MAX)/MIN);
double r = ((double)(rand()%(int)(k*10000)))/10000;
int size= (int)((double)MAX/exp(r));
return size;
}
