#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/eventfd.h>
#include <inttypes.h>

int main(int argc, char *argv[]) {
  unsigned int initval;
  int fd, flags;
  uint64_t u,v;
  ssize_t nr;
  pid_t pid;

  initval = 0;
  flags = 0;
  fd = eventfd(initval, flags);
  if (fd < 0) {
    fprintf(stderr,"eventfd: %s\n", strerror(errno));
    goto done;
  }

  pid = fork();
  if (pid < 0) {
    fprintf(stderr,"fork: %s\n", strerror(errno));
    goto done;
  }

  if (pid > 0) { /* parent */

    sleep(1);
    fprintf(stderr, "parent: writing\n");

    u = 100;
    nr = write(fd, &u, sizeof(u));
    if (nr < 0) {
      fprintf(stderr,"write: %s\n", strerror(errno));
      goto done;
    }

    wait(NULL);

  } else {       /* child */

    fprintf(stderr, "child: reading\n");
    nr = read(fd, &v, sizeof(v));
    if (nr < 0) {
      fprintf(stderr,"read: %s\n", strerror(errno));
      goto done;
    }

    fprintf(stderr, "child: read %lu\n", (unsigned long)v);
  }

 done:
  return 0;
}
