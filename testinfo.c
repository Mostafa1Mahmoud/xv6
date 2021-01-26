#include "types.h"
#include "user.h"
#include "pstat.h"

int main(int argc, char *argv[]){
  struct pstat ps;
  int in = -1;
  if (argc != 1){
  in = atoi(argv[1]);
  }
  if (in < 0) {
      while(1){
    getpinfo(&ps);
    printf(1, "\nPID\t|\tUSED?\t|\tTickets\t|\tticks\n");
    for (int i = 0; i < NPROC; i++)
    {
      if (ps.inuse[i])
        printf(1, "%d\t|\t%d\t|\t%d\t|\t%d\n", ps.pid[i], ps.inuse[i], ps.tickets[i], ps.ticks[i]);
    }
    sleep(150);
  }
  }
  else {    
          while(in--){
           getpinfo(&ps);
           printf(1, "\nPID\t|\tUSED?\t|\tTickets\t|\tticks\n");
          for (int i = 0; i < NPROC; i++)
             {
           if (ps.inuse[i])
        printf(1, "%d\t|\t%d\t|\t%d\t|\t%d\n", ps.pid[i], ps.inuse[i], ps.tickets[i], ps.ticks[i]);
    }
    sleep(150);
  }
  }
  exit();
  return 0;
}
