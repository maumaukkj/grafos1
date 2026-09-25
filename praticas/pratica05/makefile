CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o coloracao.o
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

main.o: main.c coloracao.h
	$(CC) $(CFLAGS) -c main.c

coloracao.o: coloracao.c coloracao.h
	$(CC) $(CFLAGS) -c coloracao.c

clean:
	rm -f *.o $(EXEC)