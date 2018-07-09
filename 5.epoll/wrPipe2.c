/* Program to write data to a named pipe */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PIPE "ptwo"

int main(void)
{
  int fd, nbytes = 0;
  char buff[] = "High Hopes";

  fd = open(PIPE, O_RDWR);
  if(-1 == fd)
  {
    perror("Open Fail");
    return -1;
  }
  
  nbytes = write(fd, buff, sizeof(buff));
  if(-1 == nbytes)
  {
    perror("Write Fail");
    return -1;
  }
  printf("Written %d bytes\r\n", nbytes);
  return 0;
}
