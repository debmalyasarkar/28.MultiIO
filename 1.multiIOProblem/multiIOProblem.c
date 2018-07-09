/* Program to demonstrate the problem of doing multiple IO Operations */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PIPE1 "pone"
#define PIPE2 "ptwo"

int main(void)
{
  int fd1, fd2, nbytes = 0;
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
 
  printf("Trying Read Operation %s\r\n",PIPE1);
  nbytes = read(fd1, buff, sizeof(buff));
  if(-1 == nbytes)
  {
    perror("Read Failed for First Pipe");
    return -1;
  }
  printf("Read %d bytes from %s = %s\r\n", nbytes, PIPE1, buff);

  printf("Trying Read Operation %s\r\n",PIPE2);
  nbytes = read(fd2, buff, sizeof(buff));
  if(-1 == nbytes)
  {
    perror("Read Failed for Second Pipe");
    return -1;
  }
  printf("Read %d bytes from %s = %s\r\n", nbytes, PIPE2, buff);
  return 0;
}
