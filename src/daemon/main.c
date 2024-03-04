#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

// Function to transform main process into a daemon
static void become_daemon() {
  pid_t child_pid;

  child_pid = fork();

  if (child_pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_pid > 0) {
    exit(EXIT_SUCCESS);
  }

  if (setsid() < 0) {
    exit(EXIT_FAILURE);
  }

  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  child_pid = fork();

  if (child_pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_pid > 0) {
    exit(EXIT_SUCCESS);
  }

  chdir("/");

  umask(0);

  for (int fd = sysconf(_SC_OPEN_MAX); fd >= 0; fd--) {
    close(fd);
  }

  openlog("mydaemon", LOG_PID, LOG_DAEMON);
}

int main() {
  become_daemon();

  while (1) {
    syslog(LOG_NOTICE, "mydaemon started");
    sleep(20);
    break;
  }

  syslog(LOG_NOTICE, "mydaemon terminated");
  closelog();

  return EXIT_SUCCESS;
}
