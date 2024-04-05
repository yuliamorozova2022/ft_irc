
SRCS =	main.cpp Server.cpp Channel.cpp Client.cpp PollManager.cpp \
		tools.cpp commands.cpp replies.cpp \
		mode_command.cpp

# NOTE: -w flag to hide warnings
all: ${SRCS} test_client.cpp
	c++ --std=c++98 -w ${SRCS} -o server -fsanitize=address -g
	c++ --std=c++98 -w test_client.cpp PollManager.cpp -o client
rm:
	rm -rf server
	rm -rf client

re: rm all