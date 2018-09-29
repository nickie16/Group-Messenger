FLAGS = -O3 -Wall -std=c++11

all: server client

server: TCPServer.cpp
	g++ $(FLAGS) TCPServer.cpp -o server

client: TCPClient.cpp
	g++ $(FLAGS) TCPClient.cpp -o client

