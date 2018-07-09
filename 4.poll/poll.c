/* Program to use poll() call to handle multiple IO Operations */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>

#define PIPE1 "pone"
#define PIPE2 "ptwo"

#define MAX_FD 2
#define TIMEOUT_MS 30000

int main(void)
{
  int fd1, fd2, retval, nbytes = 0;
  struct pollfd pollarray[MAX_FD];
  char buff[100] = {0};

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

  /* Initialize file descriptors to be watched into pollfd instances */  
  pollarray[0].fd = fd1;
  pollarray[1].fd = fd2;
  /* Register event type to be watched */
  /* POLLIN  --> Specifies Read Event */
  /* POLLOUT --> Specifies Write Event */
  pollarray[0].events = POLLIN;
  pollarray[1].events = POLLIN;

  /* Setup watch */
  /* Returns the number of descriptors for which event is occured */
  retval = poll(pollarray, MAX_FD, TIMEOUT_MS);
  if(retval < 0)
  {
    perror("Poll Failed");
    return -1;
  }

  /* Test whether fd1 is ready with the read event */
  if((pollarray[0].revents & POLLIN) == 1)
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

  /* Test whether fd2 is ready with the read event */
  if((pollarray[1].revents & POLLIN) == 1)
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
  /* Should also check revents signifying error cases in real applications */
  return 0;
}
