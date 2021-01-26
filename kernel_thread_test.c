#include "types.h"
#include "user.h"

void thread_test3(){
	printf(1," Hello, I am Thread 3 \n");
	exit();
}
void thread_test2(){
	printf(1," Hello, I am Thread 2 \n");
   thread_create(thread_test3,(void *)(0),(void *)(0));
   thread_join();
  exit();
}
void thread_test1(){
	printf(1," Hello, I am Thread 1 \n");
   thread_create(thread_test2,(void *)(0),(void *)(0));
   thread_join();
	exit();
}


int main(int argc, char *argv[])
{
  printf(1, "Start of Kernel Thread Test\n");
  thread_create(thread_test1,(void *)(0),(void *)(0));
  thread_join();
  thread_create(thread_test3,(void *)(0),(void *)(0));
  thread_join();
  printf(1, "End of Kernel Thread Test \n");
  exit();
}
