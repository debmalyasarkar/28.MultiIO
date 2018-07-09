/* Program to use select() call to handle multiple IO Operations */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define PIPE1 "pone"
#define PIPE2 "ptwo"

int main(void)
{
  int fd1, fd2, retval, nbytes = 0;
  fd_set read_set, write_set, exception_set;
  struct timeval timeout;
  char buff[100] = {0};

  /* 5 Second Timer */
  timeout.tv_sec  = 30;
  timeout.tv_usec = 0;

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
  
  /* Clear the fd_set */
  FD_ZERO(&read_set);
  /* Add the first fd to the fd_set */
  FD_SET(fd1, &read_set);
  /* Add the second fd to the fd_set */
  FD_SET(fd2, &read_set);

  /* Select is a blocking call which returns when one fd out of the fd_set
     performs an operation or when timeout is over */
  /* If timeout is NULL then blocks indefinitely */
  /* Can handle three independent fd_sets for read, write and exceptions */
  /* If fd from its fd_set is closed from another thread,then behaviour is 
     undefined */
  /* Here one read does not get blocked because of the other.
     Whichever fd triggers the event is processed. */
  /* Returns the number of fds for which event has occured */
  retval = select(FD_SETSIZE, &read_set, NULL, NULL, &timeout);
  if(retval < 0)
  {
    perror("Select Failed");
    return -1;
  }
  /* Display how much time is still left for timer to expire */
  printf("Time Left %ld\r\n", timeout.tv_sec);
 
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
  return 0;
}
