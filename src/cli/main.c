#include <stdio.h>
#include <string.h>

typedef struct {
    char name[50];
    void (*function)(char**);
} Command;

void help(char **args) {
    puts("Welcome to stockctl");
}

int main(int argc, char **argv) {
    Command commands[] = {
        {
            .name="help",
            .function=&help
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

    return 0;
}
