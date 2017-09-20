#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>

struct {
  int verbose;
  char *file; /* unix domain socket */
  char *prog;
  int err_fd;
  int out_fd;
} cfg = {
	.file = "log.sk",
  .err_fd = -1,
  .out_fd = -1,
};

void usage() {
  fprintf(stderr,"usage: %s [-v] -f <socket> cmd [args]\n", cfg.prog);
  exit(-1);
}

int socket_on(int dst_fd) {
  struct sockaddr_un addr;
  int sc, fd, rc = -1;

  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd == -1) {
    fprintf(stderr,"socket: %s\n", strerror(errno));
    goto done;
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, cfg.file, sizeof(addr.sun_path)-1);

  sc = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
  if (sc == -1) {
    fprintf(stderr,"connect: %s\n", strerror(errno));
    goto done;
  }

  sc = dup2(fd, dst_fd);
  if (sc < 0) {
    fprintf(stderr,"dup2: %s\n", strerror(errno));
    goto done;
  }

  sc = close(fd);
  if (sc < 0) {
    fprintf(stderr,"close: %s\n", strerror(errno));
    goto done;
  }

  rc = 0;

 done:
  return rc;
}

int main(int argc, char *argv[]) {
  int opt, rc=-1, sc, i;
  cfg.prog = argv[0];
  char **args = NULL;
  pid_t pid;

  while ( (opt = getopt(argc,argv,"vhf:")) > 0) {
    switch(opt) {
      case 'v': cfg.verbose++; break;
      case 'f': cfg.file=strdup(optarg); break;
      case 'h': default: usage(); break;
    }
  }

  if (cfg.file == NULL) usage();
  if (optind >= argc) usage();

  /* copy argv ensuring its final element is NULL */
  args = (char**)calloc(argc+1, sizeof(char*));
  if (args == NULL) goto done;
  for(i=0; i < argc; i++) args[i] = argv[i];
  args[argc] = NULL;

  fprintf(stderr, "connecting stdout/stderr to %s ", cfg.file);
  fprintf(stderr, "then executing %s\n", args[optind]);

  if (socket_on(STDOUT_FILENO) < 0) goto done;
  if (socket_on(STDERR_FILENO) < 0) goto done;

  sc = execvp(args[optind], &args[optind]);
  if (sc < 0) {
    fprintf(stderr,"execvp: %s\n", strerror(errno));
    goto done;
  }

  // NOTREACHED
  assert(0);
  rc = 0;
 
 done:
  // REACHED ON ERROR ONLY
  if (args) free(args);
  if (cfg.err_fd != -1) close(cfg.err_fd);
  if (cfg.out_fd != -1) close(cfg.out_fd);
  return rc;
}
