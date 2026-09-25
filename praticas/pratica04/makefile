CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o conectividade.o planaridade.o
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

main.o: main.c conectividade.h planaridade.h
	$(CC) $(CFLAGS) -c main.c

conectividade.o: conectividade.c conectividade.h
	$(CC) $(CFLAGS) -c conectividade.c

planaridade.o: planaridade.c planaridade.h conectividade.h
	$(CC) $(CFLAGS) -c planaridade.c

clean:
	rm -f *.o $(EXEC)