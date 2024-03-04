CC=gcc
CFLAGS= -Wall -Wextra
CLI_SRC=src/cli/main.c
CLI_BIN=bin/stockctl
DAEMON_SRC=src/daemon/main.c
DAEMON_BIN=bin/stockmgr

build: init daemon cli

cli:
	$(CC) $(CFLAGS) -o $(CLI_BIN) $(CLI_SRC)

daemon:
	$(CC) $(CFLAGS) -o $(DAEMON_BIN) $(DAEMON_SRC)

init:
	if [ -d "bin/" ]; then true; else mkdir bin/; fi

lint:
	astyle -r *.c
	cppcheck .

clean:
	rm -r ${BIN}

