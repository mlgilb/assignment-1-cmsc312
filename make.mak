CC = g++
CFLAGS = -Wall -lrt

MQCLIENT = MQclient/client-1
MQSERVER = MQserver/server-1

all: client server

client: MQclient/client-1.cpp
	$(CC) $(CFLAGS) MQclient/client-1.cpp -o $(MQCLIENT)

server: MQserver/server-1.cpp
	$(CC) $(CFLAGS) MQserver/server-1.cpp -o $(MQSERVER)

clean:
	rm -f MQclient/client-1 MQserver/server-1
