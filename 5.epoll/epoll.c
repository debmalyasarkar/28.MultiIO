/* Program to use epoll() call to handle multiple IO Operations */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/epoll.h>

#define PIPE1 "pone"
#define PIPE2 "ptwo"

#define MAX_EVENTS 10
#define TIMEOUT_MS 30000
#define SIZE 4096

int main(void)
{
  int fd1, fd2, epollfd, ii, nEvents = 0, nbytes = 0;
  char buff[100] = {0};
  /* Used for initially registering the fds to epoll instance */
  struct epoll_event ev;
  /* Used for storing return events from epoll instance */
  struct epoll_event evlist[MAX_EVENTS];

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

  /* Create an instance of epoll instance and return its file descriptor */
  epollfd = epoll_create(SIZE);
  if(-1 == epollfd)
  {
    perror("Epoll_Create Failed");
    return -1;
  }

  /* Saving file descriptor to private field to identify on return */
  ev.data.fd = fd1;
  /* Adding event type i.e. Input Event to the epoll event structure */
  ev.events  = EPOLLIN;

  /* Add epoll_event and fd to the epoll instance */
  /* Adds to internal waitqueue of eventpoll structure */
  if(-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, fd1, &ev))
  {
    perror("Epoll_Create Failed");
    return -1;
  }
  
  /* The epoll_event structure can be reused */

  /* Saving file descriptor to private field to identify on return */
  ev.data.fd = fd2;
  /* Adding event type i.e. Input Event to the epoll event structure */
  ev.events  = EPOLLIN;
  /* Add epoll_event and fd to the epoll instance */
  /* Adds to internal waitqueue of eventpoll structure */
  if(-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, fd2, &ev))
  {
    perror("Epoll_Create Failed");
    return -1;
  }

  printf("Blocked on epoll_wait\r\n");
  /* epoll_wait() is used to wait for events to occur from the epoll instance. 
     The results will be stored in the events array up to MAX_EVENTS with a 
     timeout.
     The return value of epoll_wait() indicates how many members of the events 
     array were filled with event data. */
  nEvents = epoll_wait(epollfd, evlist, MAX_EVENTS, TIMEOUT_MS);
  if(nEvents < 0)
  {
    perror("Epoll_Wait Failed");
    return -1;
  } 
  printf("No of Events Occured %d\r\n",nEvents);

  for(ii = 0; ii < nEvents; ii++)
  {
    /* In the epoll_event array where the events are saved, 
       check how many events match Input Event i.e. EPOLLIN */
    if(evlist[ii].events & EPOLLIN) 
    {
      nbytes = read(evlist[ii].data.fd, buff, sizeof(buff));
      if(-1 == nbytes)
      {
        perror("Read Failed");
        return -1;
      }
      if(evlist[ii].data.fd == fd1)
        printf("Read %d bytes from %s = %s\r\n", nbytes, PIPE1, buff);
      else if(evlist[ii].data.fd == fd2)
        printf("Read %d bytes from %s = %s\r\n", nbytes, PIPE2, buff);
    }
  }
  
  close(epollfd);
  return 0;
}

