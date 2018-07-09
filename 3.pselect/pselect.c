/* Program to use signals with pselect call to handle multiple IO Operations */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#define PIPE1 "pone"
#define PIPE2 "ptwo"

/* Common handler for all registered signals */
void signal_handler(int signum)
{
  printf("Handler\r\n");
}

int main(void)
{
  int fd1, fd2, retval, nbytes = 0;
  fd_set read_set;
  struct timespec timeout;
  struct sigaction action;
  sigset_t mask;
  char buff[100] = {0};

  /* 30 sec timeout */
  timeout.tv_sec  = 30;
  timeout.tv_nsec = 0;

  sigemptyset(&mask); 
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGQUIT);

  action.sa_handler = signal_handler;
  action.sa_mask    = mask;

  /* Register the Signals */
  if(sigaction(SIGINT, &action, NULL) < 0)
  {
    perror("SIGINT Failed");
    return -1;
  }

  if(sigaction(SIGQUIT, &action, NULL) < 0)
  {
    perror("SIGQUIT Failed");
    return -1;
  }

  fd1 = open(PIPE1 ,O_RDWR);
  if(-1 == fd1)
  {
    perror("Open Failed for First Pipe");
    return -1;
  }

  fd2 = open(PIPE2, O_RDWR);
  if(-1 == fd2)
  {
    perror("Open Failed for Second Pipe");
    return -1;
  }

  /* Infinite Loop to Handle Continuous Reads */
  while(1)
  {  
    /* Clear the fd_set */
    FD_ZERO(&read_set);
    /* Add the first fd to the fd_set */
    FD_SET(fd1, &read_set);
    /* Add the second fd to the fd_set */
    FD_SET(fd2, &read_set);

    /* pselect is a blocking call which returns when one fd out of the fd_set
       performs an operation, a signal is received or when timeout is over */
    /* Calls select() internally */
    /* If timeout is NULL then blocks indefinitely */
    /* Can handle three independent fd_sets for read, write and exceptions */
    /* If fd from its fd_set is closed from another thread,then behaviour is 
       undefined */
    /* Here one read does not get blocked because of the other.
       Whichever fd triggers the event is processed. */
    /* Returns the number of fds for which event has occured */
    retval = pselect(FD_SETSIZE, &read_set, NULL, NULL, &timeout, &mask);
    if(retval < 0)
    {
      perror("pSelect Failed");
      return -1;
    }
    
    /* After select has returned we need to find which fd has triggered,
       so we test each fd with FD_ISSET and perform our operation */
    if(FD_ISSET(fd1, &read_set))
    {
      printf("Read Operation %s\r\n",PIPE1);
      nbytes = read(fd1, buff, sizeof(buff));
      if(-1 == nbytes)
      {
        perror("Read Failed for First Pipe");
        return -1;
      }
      printf("Read %d bytes from %s = %s\r\n", nbytes, PIPE1, buff);
    }
    if(FD_ISSET(fd2, &read_set))
    {
      printf("Read Operation %s\r\n",PIPE2);
      nbytes = read(fd2, buff, sizeof(buff));
      if(-1 == nbytes)
      {
        perror("Read Failed for Second Pipe");
        return -1;
      }
      printf("Read %d bytes from %s = %s\r\n", nbytes, PIPE2, buff);
    }
  }
  return 0;
}

/* 
NOTE: This function exists to provide signal masking facility. 
      It means if a signal is received it can be masked (supressed).
*/
