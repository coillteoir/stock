CC=gcc
CFLAGS= -Wall -Wextra -Werror
CLI_SRC=src/cli/*.c
CLI_BIN=bin/stockctl
DAEMON_SRC=src/daemon/*.c
DAEMON_BIN=bin/stockmgr

build: init daemon cli

cli:
	$(CC) $(CFLAGS) -o $(CLI_BIN) $(CLI_SRC)

daemon:
	$(CC) $(CFLAGS) -o $(DAEMON_BIN) $(DAEMON_SRC)

init:
	if [ -d "bin/" ]; then true; else mkdir bin/; fi


run:
	./$(BIN) $(TEST) $(OUT)



lint:
	cppcheck .

clean:
	rm ${BIN}

