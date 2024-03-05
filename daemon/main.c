#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "../config.h"

#include "util.c"

extern int errno;

void backup() {
    //lock upload and dashboard dirs
    //copy files over to dashboard directory

    //copy files to backup directory
    //create log of what has happened.
}

int main() {
    become_daemon();

    FILE* logfile = (access(LOG_PATH, F_OK))?
                    fopen(LOG_PATH,"w+"):
                    fopen(LOG_PATH, "a");

    //makes the file stream unbuffered
    setvbuf(logfile, NULL, _IONBF, 0);

    if(mkfifo(FIFO_NAME, 0666) == -1) {
        switch(errno) {
        case EACCES:
            log_error(logfile, "Permission denied");
            break;
        default:
            log_error(logfile,"Error could not make file stream pipe");
        }
        exit(1);
    }
    int fifo_fd = open(FIFO_NAME, O_RDONLY);

    char pipe_buffer[100];
    time_t last_health_log = 0;
    time_t current = time(NULL);

    for(;;) {
        size_t bytes_read = read(fifo_fd, pipe_buffer, sizeof(pipe_buffer));

        if(bytes_read == -1) {
            log_error(logfile, "Cannot read pipe");
        }

        if(!strcmp(pipe_buffer, "stop")) {
            log_info(logfile, "Recieved signal to stop daemon.. exiting gracefully");
            break;
        }
        if(!strcmp(pipe_buffer, "backup")) {
            log_info(logfile, "Backing up files");
        }

        current = time(NULL);
        if(current - last_health_log >= 20) {
            log_info(logfile, pipe_buffer);
            log_info(logfile, "Manager healthy");

            last_health_log = current;
        }
        usleep(200000);
    }

    fclose(logfile);
    close(fifo_fd);
    unlink(FIFO_NAME);

    return EXIT_SUCCESS;
}
