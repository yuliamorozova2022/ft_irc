
SRCS =	main.cpp Server.cpp Channel.cpp Client.cpp PollManager.cpp \
		tools.cpp commands.cpp privmsg_command.cpp replies.cpp \
		mode_command.cpp

# NOTE: -w flag to hide warnings
all:
	c++ --std=c++98 -w ${SRCS} -o server
	c++ --std=c++98 -w test_client.cpp PollManager.cpp -o client
rm:
	rm -rf server
	rm -rf client

re: rm all