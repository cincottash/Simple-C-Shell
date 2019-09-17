#include "sh.h"
#include <signal.h>
#include <stdio.h>
 

void sig_handler(int signal);

int main( int argc, char **argv, char **envp )
{
  signal (SIGTSTP, sig_handler);

  return sh(argc, argv, envp);
}

void sig_handler(int sig)
{
  if(sig == SIGTSTP){
    fflush(stdout);
  }

}

