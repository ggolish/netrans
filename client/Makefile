CFLAGS = -Wall -g -Iinclude -I../common/include
CC = gcc

ifeq ($(DEBUG), 1)
    CFLAGS += -D NETRANS_DEBUG
endif


.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CFLAGS) $*.c -o $*.o

OBJS =                        \
	src/main.c	      \
	src/client.c	      \
	../common/src/error.c \
	src/args.c \
	../common/src/chunk.c \
	../common/src/common.c \
	../common/src/send.c

TARGET = netrans

default: $(TARGET)

$(TARGET): $(OBJS:.c=.o)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

send.o: ../send/src/send.c ../send/include/send.h

common.o: ../common/src/common.c ../common/include/common.h

chunk.o: ../common/src/chunk.c ../common/include/chunk.h ../common/include/packet.h

args.o: src/args.c include/args.h ../common/include/error.h

main.o: src/main.c ../common/include/error.h include/client.h include/args.h

client.o: src/client.c include/client.h ../common/include/error.h ../common/include/packet.h ../common/include/chunk.h

error.o: ../common/src/error.c ../common/include/error.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o
	rm -f ../common/src/*.o
	rm -f $(TARGET)
