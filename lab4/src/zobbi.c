#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

//#define _POSIX_SOURCE
#include <signal.h>//!!!!


#include <getopt.h>
// два процесса - родитель и потомок печатаю их PIDы. Потомок убивается по SIGTERM. При убийстве потомка не должно остаться процессов-зомби(специально).
int main(int argc, char* argv[]){
  
  pid_t cpid;
  //int status;

  cpid = fork();

  if (cpid == -1) return 1;

  if (cpid == 0) {
    
    printf("ch%d",getpid());
    

    signal(SIGTERM, SIG_DFL);
    signal(SIGABRT, SIG_IGN);

    while(1) sleep(1);

  }
  else {
    
    printf("par%d",getpid());
    

    signal(SIGTERM, SIG_IGN);
    signal(SIGABRT, SIG_IGN);

    //wait(&status);
  }

  return 0;
}