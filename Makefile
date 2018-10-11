all: client/netrans server/netransd

client/netrans:
	make -C client

server/netransd:
	make -C server

clean:
	make -C client clean
	make -C server clean
