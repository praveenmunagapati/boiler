#include <syslog.h>
#include <stdio.h>

/******************************************************************************
 * example of using syslog to log messages
 *
 *  usage: ./syslogger <message>
 *
 *  Then look for the message in one of the typical log files:
 *
 *    sudo tail /var/log/syslog
 *    sudo tail /var/log/messages
 *
 *  You should see a line like:
 *
 *    Aug 29 05:38:10 ip-172-31-27-34 ./syslogger[3159]: example syslog message
 *
 *****************************************************************************/
int main(int argc, char * argv[]) {
  char *msg = (argc > 1) ? argv[1]: "example syslog message";

  int option = LOG_PERROR | LOG_PID; /* for openlog */
  int facility = LOG_DAEMON;      /* forms syslog priority (facility|level) */
  int level = LOG_INFO;           /* forms syslog priority (facility|level) */
  int priority = facility | level;

  openlog(argv[0], option, facility);
  syslog(priority , msg);         /* this is the only required line! */
  closelog();
}
