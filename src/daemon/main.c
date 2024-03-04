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
}

int main() {
    become_daemon();
    FILE* output = fopen("/home/david/college/systems-software/log.log","w+");
    fputs("HELLO FROM DAEMON", output);
    fclose(output);

    return EXIT_SUCCESS;
}
