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

FILE* logfile;

struct dirent** load_files(DIR * dir) {
    size_t file_count = 0;
    for(struct dirent* entry=readdir(dir); entry != NULL; entry = readdir(dir)) {

        switch(entry->d_type) {
        case DT_REG:

            log_info(logfile, entry->d_name);

            file_count++;

            break;
        default:
            continue;
        }
    }

    struct dirent **files = calloc(file_count, sizeof(struct dirent *));
    if(files == NULL) {
        log_error(logfile, "cannot allocate memory for files");
        return NULL;
    }

    for(struct dirent* entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        static int i = 0;
        switch(entry->d_type) {
        case DT_REG:
            log_info(logfile, entry->d_name);

            files[i] = entry;
            i++;

            break;
        }
    }
    return files;
}

//lock upload and dashboard dirs
void backup(const char *source, const char *dest) {

    //copy files over to dashboard directory
    DIR* sourceDir = opendir(source);
    if(sourceDir == NULL) {
        char logbuf[100];
        sprintf(logbuf, "could not open file %s", source);
        log_error(logfile, logbuf);
        switch (errno) {
        case ENOENT:
            log_error(logfile, "file not found");
        }
    }

    DIR* destDir = opendir(dest);
    if(destDir == NULL) {
        char logbuf[100];
        sprintf(logbuf, "could not open file %s", dest);
        log_error(logfile, logbuf);
        switch (errno) {
        case ENOENT:
            log_error(logfile, "file not found");
        }
    }

    struct dirent **sourceFiles = load_files(sourceDir);

    closedir(destDir);
    closedir(sourceDir);

    //copy files to backup directory
    //create log of what has happened.
}

int main() {
    become_daemon();

    logfile = (access(LOG_PATH, F_OK))?
              fopen(LOG_PATH,"w+"):
              fopen(LOG_PATH, "a");

    setvbuf(logfile, NULL, _IONBF, 0);

    log_info(logfile, "logger iniialized");

    if(mkfifo(FIFO_NAME, 0666) == -1) {
        switch(errno) {
        case EACCES:
            log_error(logfile, "could not create named pipe, permission denied");
            break;
        default:
            log_error(logfile,"could not make file stream pipe");
        }
        fprintf(logfile, "ERRNO: %d\n", errno);
        exit(1);
    }

    log_info(logfile, "stock_pipe created for IPC");

    // Opening in Read/Write mode prevents the daemon from hanging while waiting
    // for the cli to open the pipe in write mode.
    int fifo_fd = open(FIFO_NAME, O_RDWR);
    if(fifo_fd == -1) {
        switch(errno) {
        case EBADF:
            log_error(logfile,"invalid path name");
            break;
        case EACCES:
            log_error(logfile, "could not open pipe, permission denied");
            break;
        default:
            log_error(logfile, "could not open pipe for reading");
        }
    }
    fcntl(fifo_fd, F_SETFL, O_NONBLOCK);
    log_info(logfile, "pipe opened for reading");

    char pipe_buffer[100];
    time_t last_health_log = 0;

    for(;;) {
        int bytes_read = read(fifo_fd, pipe_buffer, sizeof(pipe_buffer));

        if(bytes_read == -1) {
            switch (errno) {
            case EBADF:
                log_error(logfile, "invalid file descriptor, not open for reading.");
                break;
            case EAGAIN:
                // This is mainly caused by the pipe having no data.
                break;
            default:
                log_error(logfile, "cannot read pipe");
            }
        } else {
            log_info(logfile, pipe_buffer);
        }

        if(!strcmp(pipe_buffer, "stop")) {
            log_info(logfile, "Recieved signal to stop daemon.. exiting gracefully");
            break;
        }
        if(!strcmp(pipe_buffer, "backup")) {
            backup(DASH_DIR, BACKUP_DIR);
        }

        time_t current = time(NULL);
        if(current - last_health_log >= 20) {

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
