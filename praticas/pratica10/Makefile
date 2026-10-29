CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
TARGET = main
SRCS = main.c hamiltoniano.c tps.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c hamiltoniano.h tps.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all run clean