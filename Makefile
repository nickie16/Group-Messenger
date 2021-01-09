FLAGS = -O3 -Wall -std=c++11

all: server client

server: TCPServer.cpp Group.cpp ControlMessage.cpp ClientEntry.cpp
	g++ $(FLAGS) TCPServer.cpp Group.cpp ControlMessage.cpp ClientEntry.cpp -lboost_serialization -pthread -o server

client: TCPClient.cpp client.cpp ControlMessage.cpp GroupMessage.cpp ClientEntry.cpp
	g++ $(FLAGS) client.cpp TCPClient.cpp ControlMessage.cpp GroupMessage.cpp ClientEntry.cpp -lboost_serialization -pthread -o client

clean:
	rm server client

