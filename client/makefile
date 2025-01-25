# CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
# LDFLAGS:=-lboost_system -lpthread
# # TODO change the makefile for StompEMIlient
# all: EchoClient

# EchoClient: bin/ConnectionHandler.o bin/echoClient.o
# 	g++ -o bin/EchoClient bin/ConnectionHandler.o bin/echoClient.o $(LDFLAGS)

# StompWCIClient: bin/ConnectionHandler.o bin/StompClient.obin/event.o
# 	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o $(LDFLAGS)

# bin/ConnectionHandler.o: src/ConnectionHandler.cpp
# 	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

# bin/echoClient.o: src/echoClient.cpp
# 	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

# bin/event.o: src/event.cpp
# 	g++ $(CFLAGS) -o bin/event.o src/event.cpp

# bin/StompClient.o: src/StompClient.cpp
# 	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

# .PHONY: clean
# clean:
# 	rm -f bin/*
CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: StompClient

StompClient: bin/ConnectionHandler.o bin/KeyboardReader.o bin/SocketReader.o bin/StompClient.o bin/event.o bin/StompFrame.o
	g++ -o bin/StompClient bin/ConnectionHandler.o bin/KeyboardReader.o bin/SocketReader.o bin/StompClient.o bin/event.o bin/StompFrame.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/KeyboardReader.o: src/KeyboardReader.cpp
	g++ $(CFLAGS) -o bin/KeyboardReader.o src/KeyboardReader.cpp

bin/SocketReader.o: src/SocketReader.cpp
	g++ $(CFLAGS) -o bin/SocketReader.o src/SocketReader.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/StompFrame.o: src/StompFrame.cpp
	g++ $(CFLAGS) -o bin/StompFrame.o src/StompFrame.cpp

.PHONY: clean
clean:
	rm -f bin/*
