all: 
	g++ -Wall -o server server.cpp -I../src/ src/TCPServer.cpp src/TCPClient.cpp -std=c++11 -lpthread
	g++ -Wall -o client client.cpp sys_monitor.cpp jsonparser.cpp -I../src/ src/TCPServer.cpp src/TCPClient.cpp -std=c++11 -lpthread -ljansson
