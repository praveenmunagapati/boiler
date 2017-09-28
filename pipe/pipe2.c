#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define R  0
#define W  1

int main(int argc, char *argv[]) {
  char buf[100];
  int fd[2], sc;
  ssize_t nr;

  sc = pipe(fd);
  if (sc < 0) {
    fprintf(stderr,"pipe: %s\n", strerror(errno));
    exit(-1);
  }

  fprintf(stderr, "to write to pipe externally, in another window run:\n");
  fprintf(stderr, "echo hello > /proc/%u/fd/%u\n", (int)getpid(), fd[W]);
  fprintf(stderr, "waiting...\n");

  nr = read(fd[R], buf, sizeof(buf));
  if      (nr <  0) fprintf(stderr,"read: %s\n", strerror(errno));
  else if (nr == 0) fprintf(stderr,"read: eof\n");
  else    fprintf(stderr, "read %.*s\n", (int)nr, buf);

  return 0;
}
