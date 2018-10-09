CFLAGS = -Wall -g -Iinclude -I../common/include
CC = gcc

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CFLAGS) $*.c -o $*.o

OBJS = \
	src/main.c	\

TARGET = netrans

default: $(TARGET)

$(TARGET): $(OBJS:.c=.o)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

*.o: src/*.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o
	rm -f $(TARGET)
