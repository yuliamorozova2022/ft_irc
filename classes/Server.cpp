#include "Server.hpp"

    // Constructors
Server::Server(const std::string name, const int serverFd, const std::string serverPass) : _name(name), _serverFd(serverFd), _serverPass(serverPass) {
    std::cout << "\e[0;33mFields Constructor called of Server\e[0m" << std::endl;
}

    // Destructor
Server::~Server() {
    for (int i = 0; i < _channels.size(); i++)
        delete(_channels[i]);
    for (int i = 0; i < _clients.size(); i++)
        delete(_clients[i]);
    std::cout << "\e[92mDestructor called of Server\e[0m" << std::endl;
}

    // Getters / Setters
std::string Server::getName() const {return _name;}

int Server::getServerFd() const {return _serverFd;}

std::string Server::getServerPass() const {return _serverPass;}

std::string Server::getWelcomeMsg() const {return _welcomeMsg;}

std::vector<Client *> Server::getClients() const {return _clients;}

std::vector<Channel *> Server::getChannels() const {return _channels;}

void Server::setWelcomeMsg(std::string welcomeMsg) {_welcomeMsg = welcomeMsg;}

    // Methods
void Server::addChannel(Channel *channel) {
    _channels.push_back(channel);
}

void Server::addChannel(std::string channelName, Client& creator) {
    _channels.push_back(new Channel(channelName, creator));
}

void Server::addChannel(std::string channelName,std::string key, Client& creator) {
    _channels.push_back(new Channel(channelName, key, creator));
}

void Server::addClient(Client *client) {
    _clients.push_back(client);
}

void Server::addClient(std::string userName,std::string nickName, int fd, std::string host) {
    _clients.push_back(new Client(userName, nickName, fd, host));
}

