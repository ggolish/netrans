CFLAGS = -Wall -g -Iinclude -I../common/include
CC = gcc

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CFLAGS) $*.c -o $*.o

OBJS = \
	src/main.c	\
	src/server.c	\
	src/args.c \
	src/queue.c \
	../common/src/error.c \
	../common/src/common.c \
	../common/src/ack.c

TARGET = netransd

default: $(TARGET)

$(TARGET): $(OBJS:.c=.o)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

ack.o: ../common/src/ack.c ../common/include/ack.h

common.o: ../common/src/common.c ../common/include/common.h

main.o: src/main.c ../common/include/error.h

server.o: src/server.c include/server.h include/queue.h ../common/include/error.h ../common/include/packet.h

error.o: ../common/src/error.c ../common/include/error.h

args.o: src/args.c include/args.h ../common/include/error.h

queue.o: src/queue.c include/queue.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o
	rm -f ../common/src/*.o
	rm -f $(TARGET)
