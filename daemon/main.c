#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "config.h"

#include "util.c"

void log_info(FILE* logfile, const char *msg) {
    const time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = 0;
    fprintf(logfile, "INFO (%s): %s\n", timeStr, msg);
}

int main() {
    become_daemon();

    FILE* logfile = (access(LOG_PATH, F_OK))?
                    fopen(LOG_PATH,"w+"):
                    fopen(LOG_PATH, "a");

    log_info(logfile, "INIT");

    fclose(logfile);

    return EXIT_SUCCESS;
}
