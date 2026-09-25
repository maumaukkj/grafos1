CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o dag.o
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

main.o: main.c dag.h
	$(CC) $(CFLAGS) -c main.c

dag.o: dag.c dag.h
	$(CC) $(CFLAGS) -c dag.c

clean:
	rm -f *.o $(EXEC)