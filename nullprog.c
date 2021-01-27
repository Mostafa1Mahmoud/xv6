#include "types.h"
#include "stat.h"
#include "user.h"
#define NULL ((void *) 0)
int
main(int argc, char *argv[])
{
   int *pi = NULL ;
   printf(1,"I will trap now: null pointer value: ");
   printf(1, " %d\n" , *pi);
   exit();
}
