#include <stdio.h>
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

void help(char **args) {
    puts("Welcome to stockctl");
}

void startDaemon(char **args) {
    puts("I will now start the daemon (wink)");
}

void backup(char **args) {
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

    char message[] = "backup";

    printf("Writing %s to pipe\n", message);

    if(write(fifo_fd, message, sizeof(message)) == -1) {
        switch (errno) {
        case EBADF:
            puts("Bad file descriptor");
            break;
        default:
            puts("Could not write to pipe");
        }
        exit(1);
    }

}

void stop(char **args) {
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

    char message[] = "stop";

    printf("Writing %s to pipe\n", message);

    if(write(fifo_fd, message, sizeof(message)) == -1) {
        switch (errno) {
        case EBADF:
            puts("Bad file descriptor");
            break;
        default:
            puts("Could not write to pipe");
        }
        exit(1);
    }

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
