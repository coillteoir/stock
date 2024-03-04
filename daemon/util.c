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
