#include <iostream>
#include <string>

#include "Server.hpp"

#define BUFF 1000
//get_command
// basically gnl
//

std::string get_command(Client &client, int &stat)
{
	std::vector<char> buffer(BUFF);
	while (client.getMsgBuffer().find('\n') == client.getMsgBuffer().npos)
	{
		std::fill(buffer.begin(), buffer.end(), 0);
		stat = recv(client.getFd(), buffer.data(), BUFF, MSG_DONTWAIT);
		if (stat == -1)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("  recv() failed");
			return "";
		}
		if (stat == 0)
			return "";
		client.addMsgBuffer(buffer.data());
	}
//keep leftovers in lient->msgbuffer

	std::string msg = client.getMsgBuffer().substr(0, client.getMsgBuffer().find_first_of('\n'));
	client.setMsgBuffer(client.getMsgBuffer().substr( client.getMsgBuffer().find_first_of('\n') + 1 ));

//return till /n
	return (msg);
}
/*
std::string get_command(Client &client, int &stat)
{
	std::vector<char> buffer(BUFF);
	std::string msg = client.getMsgBuffer();
	int bytes;

	while (msg.find('\n') == msg.npos)
	{
		std::fill(buffer.begin(), buffer.end(), 0);
		bytes = recv(client.getFd(), buffer.data(), BUFF, MSG_DONTWAIT);
		stat = bytes;
		if (bytes == -1)
		{
			std::cout << "errno: " << errno << "EWOULDBLOCK: " << EWOULDBLOCK << std::endl;
			std::cout << "msg: " << msg << std::endl;
			perror (":(");
			return "";
		}
		if (bytes == 0)
		{
			std::cout << "here" << std::endl;
			return "";
		}
		msg.append(buffer.data());
	}
//keep leftovers in lient->msgbuffer
	client.setMsgBuffer(msg.substr(msg.find_first_of('\n') + 1));

//return till /n
	return (msg.substr(0, msg.find_first_of('\n')));
} */