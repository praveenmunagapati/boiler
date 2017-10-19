#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/fcntl.h>
 
int verbose=0;
void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] <file>\n", prog);
  exit(-1);
}
 
char *map(char *file, size_t *len) {
  int fd = -1, rc = -1;
  char *buf = NULL;
  struct stat s;

  *len = 0;

  if ( (fd = open(file, O_RDONLY)) == -1) {
    fprintf(stderr,"open %s: %s\n", file, strerror(errno));
    goto done;
  }

  if (fstat(fd, &s) == -1) {
    fprintf(stderr,"fstat %s: %s\n", file, strerror(errno));
    goto done;
  }

  buf = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (buf == MAP_FAILED) {
    fprintf(stderr, "mmap %s: %s\n", file, strerror(errno));
    goto done;
  }

  rc = 0;
  *len = s.st_size;

 done:
  if (fd != -1) close(fd);
  if ((rc < 0) && (buf != NULL) && (buf != MAP_FAILED)) munmap(buf, s.st_size);
  return (rc < 0) ? NULL : buf;
}

int main(int argc, char * argv[]) {
  char *file, *buf=NULL;
  size_t len;;
  int opt;
 
  while ( (opt = getopt(argc, argv, "v+")) != -1) {
    switch (opt) {
      case 'v':
        verbose++;
        break;
      default:
        usage(argv[0]);
        break;
    }
  }
 
  if (optind < argc) file=argv[optind++];
  else usage(argv[0]);
  buf = map(file, &len);
  if (buf == NULL) goto done;

  printf("mmap'd %s at %p: %lu bytes\n", file, buf, (long unsigned)len);
 
 done:
  if (buf) munmap(buf, len);
}
