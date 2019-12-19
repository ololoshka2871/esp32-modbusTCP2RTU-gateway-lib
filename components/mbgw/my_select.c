#include <sys/select.h>

#include "log.h"

// ignore select if skip_d in __writefds, force it true
int my_select(int __nfds, fd_set *__restrict __readfds,
              fd_set *__restrict __writefds, fd_set *__restrict __exceptfds,
              struct timeval *__restrict __timeout, int skip_d) {
  if (FD_ISSET(skip_d, __writefds)) {
    __timeout->tv_sec = 0;
    __timeout->tv_usec = 0;
    logw(1, "select() waiting TX ready, forcing..");
    FD_CLR(skip_d, __writefds);
    int res = select(__nfds, __readfds, __writefds, __exceptfds, __timeout);
    FD_SET(skip_d, __writefds);
    return res + 1;
  }

  return select(__nfds, __readfds, __writefds, __exceptfds, __timeout);
}
