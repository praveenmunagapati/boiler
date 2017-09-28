#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

/* 
 * bind, listen and accept clients to our unix domai socket.
 */

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  int fd,ad,rc=-1;
  char buf[100];

  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "socket: %s\n", strerror(errno));
    goto done;
  }

  char *socket_path = "/tmp/example.sock";
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
  unlink(socket_path);

  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    fprintf(stderr, "bind: %s\n", strerror(errno));
    goto done;
  }

  if (listen(fd, 5) == -1) {
    fprintf(stderr, "listen: %s\n", strerror(errno));
    goto done;
  }

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

