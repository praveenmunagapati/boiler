#Example of using syslog to log messages

See syslogger.c. This C example is a workalike of logger(1). Usage:

    % ./syslogger <message>

Look for the log message in one of the typical log files:

    % sudo tail /var/log/syslog
    % sudo tail /var/log/messages

You should see a line like:

    Aug 29 05:38:10 ip-172-31-27-34 ./syslogger[3159]: example syslog message

Internally, on the host used to run this example, the daemon `rsyslogd`
received the log messages, through the Unix domain socket `/dev/log`.  The
rsyslogd configuration file `/etc/rsyslog.conf` drives its behavior.

# API

This the syslog(3) API:

    void syslog(int priority, const char *format, ...);

where `priority` is the logical OR of 

    <facility> | <level>

E.g.,

    LOG_DAEMON | LOG_INFO

The format and the trailing arguments are like `printf`. The extra format
character `%m` is substituted with strerror(errno).

## openlog and closelog

The openlog and closelog are optional:

    void openlog(const char *ident, int option, int facility);
    void closelog(void);
 
While openlog is optional, it can be useful because of its options argument.
E.g. there are options to copy log messages to stderr (`LOG_PERROR`) and to
include the pid in the log message (`LOG_PID`). E.g.,

    openlog(argv[0], LOG_PERROR | LOG_PID, LOG_DAEMON);

The closelog function closes the file descriptor to the underlying `/dev/log`
socket.

## Internals 

The syslog functions connect and write to `/dev/log`, a a Unix domain socket.
This can be observed in the output of strace(1) when running syslogger:

    % strace ./syslogger
    ...
    socket(PF_LOCAL, SOCK_DGRAM|SOCK_CLOEXEC, 0) = 3
    connect(3, {sa_family=AF_LOCAL, sun_path="/dev/log"}, 110) = 0
    sendto(3, "<30>Aug 29 05:40:45 ./syslogger["..., 61, MSG_NOSIGNAL, NULL, 0)

We can see that `rsyslogd` is the listening process for `/dev/log`:

    % sudo ss -plx | grep /dev/log
    u_dgr  UNCONN  ...  /dev/log ... users:(("rsyslogd",pid=2230,fd=0))

## RFC 

The message format is described in [RFC 3164](https://tools.ietf.org/html/rfc3164).
Syslog messages were originally sent over UDP/514 as datagrams.  There are now
standards for transmitting syslog over TCP and TLS, standardized in more detail
in [RFC 5424](https://tools.ietf.org/html/rfc5424).

# Stdout/stderr to syslog

This is an example of using basic Unix plumbing to connect the stdout and stderr
of a program to syslog, without modifying the program.

The program `sockout.c` re-opens stdout and stderr into a Unix domain socket.
Then it executes an arbitrary user-specified program, whose output is now
directed into the socket.  A companion program, `socksrv.c`, binds the
socket and directs incoming content to syslog. The latter program must be
started first:

    % ./socksrv -f log.sock

While left running, run the example below in a second window. This example
arbitrarily uses `ls /etc` as the program whose output to send to syslog.

    % ./sockout -f log.sock ls /etc

If you now tail the syslog, you should see the output of `ls /etc` there.
