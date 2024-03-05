static void become_daemon() {
    pid_t child_pid = fork();

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

void log_generic(FILE* logfile, const char *prefix, const char *msg) {
    const time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = 0;
    fprintf(logfile, "%s (%s): %s\n", prefix, timeStr, msg);
}

void log_error(FILE* logfile, const char *msg) {
    log_generic(logfile,"ERROR", msg);
}

void log_info(FILE* logfile, const char *msg) {
    log_generic(logfile,"INFO ", msg);
}
