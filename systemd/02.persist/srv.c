#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

/* this server is designed to run under systemd (only),
   it expects systemd to exec this process with the 
   listening socket already opened on fd 3.
   our job is to accept it and handle the client.
   afterward, we close the client. 

   this version of the server stays running, accepting
   new client connections, rather than letting systemd
   restart it when a new connection comes in.
 */

int main(int argc, char *argv[]) {
  int fd,ad,rc=-1;
  char buf[100];

  /* accept on fd 3. we are hardcoding it for this example */
  fd = 3;

  while (1) {
    if ( (ad = accept(fd, NULL, NULL)) == -1) {
      fprintf(stderr, "accept: %s\n", strerror(errno));
      goto done;
    }

    while ( (rc=read(ad,buf,sizeof(buf))) > 0) {
      printf("read %u bytes: %.*s\n", rc, rc, buf);
    }

    if (rc == -1) {
      fprintf(stderr, "read: %s\n", strerror(errno));
      exit(-1);
    }
    else if (rc == 0) {
      printf("EOF\n");
      close(ad);
    }
  }

  rc = 0;

 done:
  return rc;
}

