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
	src/netrans.c	      \
	../common/src/error.c \
	src/args.c \
	../common/src/chunk.c

TARGET = netrans

default: $(TARGET)

$(TARGET): $(OBJS:.c=.o)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

chunk.o: ../common/src/chunk.c ../common/include/chunk.h ../common/include/packet.h

args.o: src/args.c include/args.h ../common/include/error.h

main.o: src/main.c ../common/include/error.h include/netrans.h include/args.h

netrans.o: src/netrans.c include/netrans.h ../common/include/error.h ../common/include/packet.h ../common/include/chunk.h

error.o: ../common/src/error.c ../common/include/error.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o
	rm -f ../common/src/*.o
	rm -f $(TARGET)
