#include <sys/mman.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "buddy.h"

#define MIN 5
#define LEVELS 8
#define PAGE 4096
 int total_memory = 0;
enum flag {Free,Taken};

struct head{
  enum flag status;
  short int level;
  struct head *next;
  struct head *prev;
};
struct head *flist[LEVELS] = {NULL};
struct head *new(){
  struct head *new = (struct head *) mmap(NULL,PAGE, PROT_READ|PROT_WRITE,
                                            MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
if(new==MAP_FAILED){
  return NULL;
}
assert(((long int)new & 0xfff)==0); //12 last bits should be zero
new->status = Taken;
new->level=LEVELS-1;
//printf("%p current address\n", new);
total_memory = total_memory + PAGE;
return new;
}

struct head *buddy(struct head* block){
  int index = block->level;
  long int mask = 0x1 << (index + MIN);
  return (struct head*)((long int)block ^ mask);
}

struct head *merge(struct head* block, struct head* sibling){
  struct head *primary;
  if(sibling<block)
    primary=sibling;
  else
    primary=block;
  primary->level = primary->level+1;
  return primary;
}

void *hide(struct head* block){
  return (void*)(block+1);
}

struct head *magic(void *memory){
  return ((struct head*)memory-1);
}

int level(int req) {
  int total = req  + sizeof(struct head);

  int i = 0;
  int size = 1 << MIN;
  while(total > size) {
    size <<= 1;
    i += 1;
  }
  return i;
}

void printBlock(struct head* block){
  printf("LEVEL: %d, FLAG: %d, ADDRESS: %p \n", block->level,block->status,block);
}

void add_block(struct head* block){
  int index = block->level;
  block->next = flist[block->level];
  block->prev=NULL;
  flist[block->level] = block;
  if(block->next!=NULL){
    block->next->prev = block;
  }
}

//LEVEL????
struct head *split(struct head *block) {
  block->status=Taken;
  int index = block->level - 1;
  int mask =  0x1 << (index + MIN);
  struct head* this = (struct head *)((long int)block | mask);
  this->level = index;
  struct head* that = buddy(this);
  that->level = index;
  that->status=Free;
  add_block(that);
  return this;
}



struct head *find(int index) {
  if(flist[index]==NULL){
  if(index==7){
    struct head* newblock = new();
    newblock->status=Taken;
    newblock->level=index;
        return newblock;
  }
  struct head* temp = find(index+1);
  temp = split(temp);
  temp->status=Taken;
  return temp;
}
    struct head* temp = flist[index];
    if(temp->next!=NULL)
      temp->next->prev=NULL;
    flist[index] = temp->next;
    temp->next=NULL;
    temp->prev=NULL;
    temp->status=Taken;
    return temp;
}
/**
void unlink(struct head* block){
  int index = block->level;
  if(block->prev==NULL && block->next==NULL)
    flist[index]=NULL;
  if(block->prev!=NULL){
    block->prev->next=block->next;
  }
  if(block->next!=NULL){
    block->next->prev=block->prev;
    if(block->prev==NULL){
      flist[index] == block->next;
    }
  }

  block->next=NULL;
  block->prev=NULL;
}**/


void unlink(struct head* block){
  int index = block->level;
  if(block->prev==NULL && block->next==NULL){
    flist[index]=NULL;
  }
  else if(block->prev!=NULL && block->next!=NULL){
    block->prev->next = block->next;
    block->next->prev=block->prev;
  }
  else if(block->prev!=NULL && block->next==NULL){
    block->prev->next = block->next;
  }
  else if(block->next!=NULL && block->prev==NULL){
    block->next->prev=block->prev;
    flist[index]=block->next;
  }
  block->next=NULL;
  block->prev=NULL;
}


/**
 void insert(struct head* block){
   block->status = Free;
   int index = block->level;
  if(index!=7){
    struct head* sibling = buddy(block);
    if(sibling->status == Free){
      unlink(sibling);
      insert(merge(block,sibling));
    }
    else{
      add_block(block);
    }
  }
  if(index==7){
    add_block(block);
  }
}**/

void insert(struct head* block){
  int index = block->level;
  block->status=Free;
  if(index!=7){
    struct head* friend = buddy(block);
    if(friend->status==Free && friend->level==index){
      unlink(friend);
      insert(merge(friend,block));
    }
    else
      add_block(block);
  }
  if(index==7){
   // munmap(block,PAGE);
   // total_memory-=PAGE;
   add_block(block);
 }
}

void bfree(void *memory) {
  if(memory != NULL) {
    struct head *block = magic(memory);
    insert(block);
  }
  return;
}



void *balloc(size_t size) {
  if( size == 0 ){
    return NULL;
  }
  int index = level(size);
  struct head *taken = find(index);
  return hide(taken);
}

int getTotal(){
	return total_memory;
}

int externalFragmentation() {
 int sum=0;
 for(int i = LEVELS-1; 0 <= i; i--) {
   if(flist[i] == NULL) {
   }
    else {
     int j = pow(2,(i+5));
     sum+=j;
     struct head *loopvar = flist[i];
     while(loopvar->next != NULL) {
       sum+=j;
       loopvar = loopvar->next;
     }
   }
 }
 return sum;
}

void test(){

/**
printf("size of head is: %ld\n", sizeof(struct head));
  printf("level for  7 should be 0 : %d\n", level(7));
  printf("level for  8 should be 0 : %d\n", level(8));
  printf("level for  9 should be 1 : %d\n", level(9));
  printf("level for  20 should be 1 : %d\n", level(20));
  printf("level for  40 should be 1 : %d\n", level(40));
printf("level for  41 should be 2 : %d\n", level(41));
**/
}
