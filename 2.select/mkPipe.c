/* Create two named pipe i.e. FIFO */
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FILE1 "pone"
#define FILE2 "ptwo"

int main(void)
{
  if(0 != mkfifo(FILE1, S_IRWXU | S_IRWXG | S_IRWXO))
  {
    perror(FILE1);
    return -1;
  }
  printf("Created Named Pipe %s\r\n",FILE1);
  if(0 != mkfifo(FILE2, S_IRWXU | S_IRWXG | S_IRWXO))
  {
    perror(FILE2);
    return -1;
  }
  printf("Created Named Pipe %s\r\n",FILE2);
  return 0;
}
