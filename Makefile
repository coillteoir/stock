CC=gcc
CFLAGS= -Wall -Wextra
CLI_SRC=cli/main.c
CLI_BIN=bin/stockctl
DAEMON_SRC=daemon/main.c
DAEMON_BIN=bin/stockmgr

build: init lint
	$(CC) $(CFLAGS) -o $(CLI_BIN) $(CLI_SRC)
	$(CC) $(CFLAGS) -o $(DAEMON_BIN) $(DAEMON_SRC)

init:
	if [ -d "bin/" ]; then true; else mkdir bin/; fi

lint:
	astyle -r *.c
	cppcheck .

clean:
	rm -fr bin/
	rm -f log.log
	rm -f stock_pipe
