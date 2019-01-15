#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void *malloc(size_t size){
  if(size==0){
    return NULL;
  }

  void *memory = sbrk(size);
  if(memory == (void *)-1){
    return NULL;
  }else{
    return memory;
  }
}

void free(void *memory){
  return;
}

int main() {
  int arr[] = {1,2,3,4};
  char arrChar[] = {'h','e','j'};
 
 printf("%p\n %p\n, %p\n", &arr, &arr[0], &arr[1]);
 printf("\n\n\n %p\n %p\n %p\n", &arrChar, &arrChar[0], &arrChar[1]);
 printf("%d", sizeof(arrChar[0]));
}
