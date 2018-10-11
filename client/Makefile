CFLAGS = -Wall -g -Iinclude -I../common/include
CC = gcc

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CFLAGS) $*.c -o $*.o

OBJS = \
	src/main.c	\
	src/netrans.c	\
	../common/src/error.c

TARGET = netrans

default: $(TARGET)

$(TARGET): $(OBJS:.c=.o)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

main.o: src/main.c ../common/include/error.h include/netrans.h

netrans.o: src/netrans.c include/netrans.h ../common/include/error.h ../common/include/packet.h

error.o: ../common/src/error.c ../common/include/error.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o
	rm -f ../common/src/*.o
	rm -f $(TARGET)
