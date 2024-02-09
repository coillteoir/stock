CC=gcc
CFLAGS= -Wall -Wextra -Werror
BIN=bin/manager
SRC=src/main.c

build:
	if [ -d "bin/" ]; then true; else mkdir bin/; fi
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) 

run:
	./$(BIN) $(TEST) $(OUT)

both: build run


lint:
	cppcheck .

clean:
	rm ${BIN}
