#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "../config.h"

extern int errno;

typedef struct {
    char name[50];
    void (*function)(char**);
} Command;

void write_fifo(char *message) {
    int fifo_fd = open(FIFO_NAME, O_WRONLY);

    if(fifo_fd == -1) {
        switch (errno) {
        case EACCES:
            printf("Could not open %s permission denied\n", FIFO_NAME);
            break;
        default:
            puts("Could not write to pipe");
        }
        exit(1);
    }

    printf("Writing %s to pipe\n", message);

    if(write(fifo_fd, message, strlen(message)) == -1) {
        switch (errno) {
        case EBADF:
            puts("Bad file descriptor");
            break;
        default:
            puts("Could not write to pipe");
        }
        exit(1);
    }
    close(fifo_fd);
}

void help() {
    puts("Welcome to stockctl");
}

bool find_process() {
    return false;
}

void startDaemon() {

    pid_t pid = fork();

    if(pid == 0) {
        system("./bin/stockmgr");
    } else if(pid > 0) {
        printf("Fork created to start stockmgr\n");
    } else {
        puts("Error, fork failed");
    }
}

void backup() {
    write_fifo("backup");
}

void stop() {
    write_fifo("stop");
}

int main(int argc, char **argv) {
    Command commands[] = {
        {
            .name="help",
            .function=&help,
        },{
            .name="start",
            .function=&startDaemon,
        },{
            .name="backup",
            .function=&backup,
        },{
            .name="stop",
            .function=&stop,
        }
    };

    if(argc == 1) {
        puts("Please specify a command\nhelp\ninit\nstop");
        return 0;
    }

    size_t command_len = sizeof(commands)/sizeof(commands[0]);
    for(size_t i = 0; i < command_len; i++) {
        if(!strcmp(argv[1], commands[i].name))
            commands[i].function(argv + 2);
    }

    return EXIT_SUCCESS;
}
