all:
	c++ --std=c++98 test_client.cpp PollManager.cpp -g -o client
	c++ --std=c++98 test_server.cpp PollManager.cpp -g -o server

rm:
	rm -rf client server

re: rm all