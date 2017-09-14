#include <sys/epoll.h>
#include <assert.h>
#include <syslog.h>
#include <sys/signalfd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>

struct {
  int verbose;
  char *file; /* unix domain socket */
  char *prog;
  int sock_fd;     /* listening descriptor */
  int epoll_fd;
  int signal_fd;
} cfg = {
	.file = "log.sk",
  .sock_fd = -1,
  .epoll_fd = -1,
  .signal_fd = -1,
};

void usage() {
  fprintf(stderr,"usage: %s [-v] -f <socket>\n", cfg.prog);
  exit(-1);
}

/* signals that we'll accept via signalfd in epoll */
int sigs[] = {SIGHUP,SIGTERM,SIGINT,SIGQUIT,SIGALRM};

int setup_listener(void) {
  struct sockaddr_un addr;
  int sc, rc = -1;

  cfg.sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (cfg.sock_fd == -1) {
    fprintf(stderr,"socket: %s\n", strerror(errno));
    goto done;
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, cfg.file, sizeof(addr.sun_path)-1);
  unlink(cfg.file);

  sc = bind(cfg.sock_fd, (struct sockaddr*)&addr, sizeof(addr));
  if (sc == -1) {
    fprintf(stderr,"bind: %s\n", strerror(errno));
    goto done;
  }

  sc = listen(cfg.sock_fd, 5);
  if (sc == -1) {
    fprintf(stderr,"listen: %s\n", strerror(errno));
    goto done;
  }

  rc = 0;

 done:
  return rc;
}

int new_epoll(int events, int fd) {
  struct epoll_event ev;
  int rc;
  memset(&ev,0,sizeof(ev)); // placate valgrind
  ev.events = events;
  ev.data.fd= fd;
  rc = epoll_ctl(cfg.epoll_fd, EPOLL_CTL_ADD, fd, &ev);
  if (rc == -1) {
    fprintf(stderr,"epoll_ctl: %s\n", strerror(errno));
  }
  return rc;
}

int periodic_work(void) {
	int rc = -1;
  rc = 0;
	return rc;
}

int handle_socket(void) {
  int sc, rc = -1;

  sc = accept(cfg.sock_fd, NULL, NULL);
  if (sc < 0) {
    fprintf(stderr,"accept: %s\n", strerror(errno));
    goto done;
  }

  // sc is a client descriptor. epoll on it
  if (new_epoll(EPOLLIN, sc)) goto done;

  rc = 0;

 done:
  return rc;
}

int handle_signal() {
  struct signalfd_siginfo info;
	ssize_t nr;
  int rc=-1;
  
  nr = read(cfg.signal_fd, &info, sizeof(info));
  if (nr != sizeof(info)) {
    fprintf(stderr,"failed to read signal fd buffer\n");
    goto done;
  }

  switch(info.ssi_signo) {
    case SIGALRM: 
      if (periodic_work() < 0) goto done;
      alarm(1); 
      break;
    default: 
      fprintf(stderr,"got signal %d\n", info.ssi_signo);  
      goto done;
      break;
  }

 rc = 0;

 done:
  return rc;
}

/* the standard output or error stream may contain multiple lines.
 * make a basic attempt to syslog the lines independently.
 * TODO sockaddr un of remote end
 */
int produce_log(char *b, ssize_t n) {
  char *l, *eol;
  l = b;
  assert(n > 0);
  do {
    /* find end of line */
    eol = l+1;
    while((eol < b+n) && (*eol != '\n')) eol++;
    syslog(LOG_DAEMON|LOG_INFO, "%.*s", (int)(eol-l), l);
    l = eol+1;
  } while(l < b+n);
}

int handle_client(int fd) {
  char buf[1000], *l;
  ssize_t nr, n;
  int rc = -1;

  nr = read(fd, buf, sizeof(buf));
  if (nr < 0) {
    fprintf(stderr,"read: %s\n", strerror(errno));
    goto done;
  }
  else if (nr == 0) { /* normal client closure */
    if (cfg.verbose) fprintf(stderr,"read: eof\n");
    close(fd);
  }
  else if (nr > 0) {
    produce_log(buf,nr);
  }

  rc = 0;

 done:
  return rc;
}

int main(int argc, char *argv[]) {
  int opt, rc=-1, sc, n;
  struct epoll_event ev;
  cfg.prog = argv[0];

  while ( (opt = getopt(argc,argv,"vhf:")) > 0) {
    switch(opt) {
      case 'v': cfg.verbose++; break;
      case 'f': cfg.file=strdup(optarg); break;
      case 'h': default: usage(); break;
    }
  }

  if (cfg.file == NULL) usage();
  if (setup_listener() < 0) goto done;

  /* block all signals. we take signals synchronously via signalfd */
  sigset_t all;
  sigfillset(&all);
  sigprocmask(SIG_SETMASK,&all,NULL);

  /* a few signals we'll accept via our signalfd */
  sigset_t sw;
  sigemptyset(&sw);
  for(n=0; n < sizeof(sigs)/sizeof(*sigs); n++) sigaddset(&sw, sigs[n]);

  /* create the signalfd for receiving signals */
  cfg.signal_fd = signalfd(-1, &sw, 0);
  if (cfg.signal_fd == -1) {
    fprintf(stderr,"signalfd: %s\n", strerror(errno));
    goto done;
  }


  /* set up the epoll instance */
  cfg.epoll_fd = epoll_create(1); 
  if (cfg.epoll_fd == -1) {
    fprintf(stderr,"epoll: %s\n", strerror(errno));
    goto done;
  }

  if (new_epoll(EPOLLIN, cfg.sock_fd)) goto done;
  if (new_epoll(EPOLLIN, cfg.signal_fd)) goto done;

  openlog(cfg.prog, LOG_PERROR|LOG_PID, LOG_DAEMON);

  alarm(1);
  while (epoll_wait(cfg.epoll_fd, &ev, 1, -1) > 0) {
    if      (ev.data.fd == cfg.signal_fd)   { if (handle_signal() < 0) goto done; }
    else if (ev.data.fd == cfg.sock_fd)     { if (handle_socket() < 0) goto done; }
    else { if (handle_client(ev.data.fd) < 0) goto done; }
  }

  rc = 0;
 
 done:
  if (cfg.sock_fd != -1) close(cfg.sock_fd);
  if (cfg.epoll_fd != -1) close(cfg.epoll_fd);
  if (cfg.signal_fd != -1) close(cfg.signal_fd);
  return rc;
}
