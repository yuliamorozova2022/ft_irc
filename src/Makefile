
all:
	c++ --std=c++98 main.cpp Server.cpp Channel.cpp Client.cpp PollManager.cpp -o server
	c++ --std=c++98 test_client.cpp PollManager.cpp -o client
rm:
	rm -rf server
	rm -rf client

re: rm all