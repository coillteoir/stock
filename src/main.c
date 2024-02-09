#include <stdio.h>
#include <unistd.h>

#define NOCHDIR 0
#define NOCLOSE 0

int main() {

    FILE * logfile = fopen("logfile.txt", "w+");

    if(daemon(NOCHDIR, NOCLOSE))
        perror("daemon is running\n");

    fputs("I am going to sleep\n", logfile);
    sleep(10);
    fputs("good morning!!\n",logfile);

    fclose(logfile);

    return 0;
}
