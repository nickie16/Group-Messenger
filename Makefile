FLAGS = -O3 -Wall -std=c++11

server: TCPServer.cpp
	g++ $(FLAGS) TCPServer.cpp -o server

client: TCPClient.cpp
	g++ $(FLAGS) TCPClient.cpp -o client

all: server client 
