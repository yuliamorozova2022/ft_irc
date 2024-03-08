FT IRC == ???

Internet Relay Chat (IRC)
	an Internet application that was developed by Jakko Oikarinen in Finland.
	IRC follows client-server model - both client and server software are required in order to use it.

	First, you have to log on to the server using a client and then pick the channel on which you want to chat. They are sent to your server when you type. Now your server is part of global IRC server network. Your server sends your messages to other servers, which in turn, sends your messages to people who are part of your channel.

	IRC has a line-based structure. Clients send single-line messages to the server, receive replies to those messages and receive copies of some messages sent by other clients. In most clients, users can enter commands by prefixing them with a '/'. Depending on the command, these may either be handled entirely by the client, or (generally for commands the client does not recognize) passed directly to the server, possibly with some modification.

	Due to the nature of the protocol, automated systems cannot always correctly pair a sent command with its reply with full reliability and are subject to guessing.[46]
	references:
	https://github.com/kala13x/derpina
	https://www.geeksforgeeks.org/internet-relay-chat-irc/
	https://ircgod.com/
	https://en.wikipedia.org/wiki/IRC#Technical_information

https://github.com/marineks/Ft_irc note: nc==netcat

https://datatracker.ietf.org/doc/html/rfc2813

https://www.youtube.com/watch?v=FDrR98ww6bE ==> netcat with IRC

https://fedoramagazine.org/beginners-guide-irc/

https://ncona.com/2019/04/building-a-simple-server-with-cpp/


IRC project documentations by the university of Chicago
http://chi.cs.uchicago.edu/chirc

So basically, the client would send the server basic commands and the server woud respond. basic. server would have to authenticate client, using the password provided in the command line.

irrsi - cli base Irc client.would be good reference
https://irssi.org/download/


Youtube Vid
Using IRC (Internet Relay Chat) - Drupalize .me
https://www.youtube.com/watch?v=Lxtnw1v5FSI


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TCPFLOW - A tool to monitor passing TCP packages
```bash
sudo tcpflow -i any -g -T %T'    '%A'::'%a'->'%B'::'%b -c port 6666 > ./tcpflow_out
```
-i:
	specifies what network interface to listen on (‘any’ is a reasonable default to start you off)
-g:
	(used to be -J)  gives information in different colors
-c:
	prints to the console instead of creating individual files. By default, TCP Flow creates two files for each TCP conversation – one file for the packets coming in and one for the packets being transmitted.
-T:
	the format of the output lines. more info in man tcpflow

plus port specification.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~











FTIRC allowed commands from the pdf + basic info thanks to chatGPT:
1. **socket**:
	```cpp
	int socket(int domain, int type, int protocol);
	```
	Creates a new communication endpoint, returning a socket descriptor for further use in network communication.

2. **close**:
	```cpp
	int close(int fd);
	```
	Closes a file descriptor, including sockets, to release system resources and terminate the connection.

3. **setsockopt**:
	```cpp
	int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	```
	Sets various options associated with a socket, such as controlling socket behavior or modifying socket properties.

4. **getsockname**:
	```cpp
	int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	```
	Retrieves the local address assigned to a socket, useful for obtaining the details of a bound socket.

5. **getprotobyname**:
	```cpp
	struct protoent *getprotobyname(const char *name);
	```
	Retrieves protocol information based on the protocol name, aiding in setting up socket options.

6. **gethostbyname**:
	```cpp
	struct hostent *gethostbyname(const char *name);
	```
	Resolves a hostname to an IPv4 address, allowing the program to identify and connect to remote hosts.

7. **getaddrinfo**:
	```cpp
	int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
	```
	Translates a host name and a service name to a set of socket addresses, providing flexibility in handling different address formats.

8. **freeaddrinfo**:
	```cpp
	void freeaddrinfo(struct addrinfo *res);
	```
	Frees memory allocated by the getaddrinfo function, preventing memory leaks.

9. **bind**:
	```cpp
	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	```
	Associates a socket with a specific local address and port number, preparing it for communication.

10. **connect**:
	```cpp
	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	```
	Establishes a connection to a remote socket, enabling communication between the local and remote systems.

11. **listen**:
	```cpp
	int listen(int sockfd, int backlog);
	```
	Marks a socket as passive, indicating that it will be used to accept incoming connection requests.

12. **accept**:
	```cpp
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	```
	Accepts an incoming connection request on a listening socket, creating a new socket for communication with the client.

13. **htons**:
	```cpp
	uint16_t htons(uint16_t hostshort);
	```
	Converts a 16-bit integer from host byte order to network byte order, ensuring consistency in communication protocols.

14. **htonl**:
	```cpp
	uint32_t htonl(uint32_t hostlong);
	```
	Converts a 32-bit integer from host byte order to network byte order.

15. **ntohs**:
	```cpp
	uint16_t ntohs(uint16_t netshort);
	```
	Converts a 16-bit integer from network byte order to host byte order.

16. **ntohl**:
	```cpp
	uint32_t ntohl(uint32_t netlong);
	```
	Converts a 32-bit integer from network byte order to host byte order.

17. **inet_addr**:
	```cpp
	in_addr_t inet_addr(const char *cp);
	```
	Converts an IPv4 address in string format to a 32-bit binary representation.

18. **inet_ntoa**:
	```cpp
	char *inet_ntoa(struct in_addr in);
	```
	Converts a 32-bit binary representation of an IPv4 address to a string format.

19. **send**:
	```cpp
	ssize_t send(int sockfd, const void *buf, size_t len, int flags);
	```
	Transmits data over a connected socket.

20. **recv**:
	```cpp
	ssize_t recv(int sockfd, void *buf, size_t len, int flags);
	```
	Receives data from a connected socket.

21. **signal**:
	```cpp
	void (*signal(int signum, void (*handler)(int)))(int);
	```
	Associates a signal handler function with a specific signal, allowing custom code to be executed upon signal reception.

22. **sigaction**:
	```cpp
	int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
	```
	A more advanced signal-handling function that provides additional control and features compared to signal.

23. **lseek**:
	```cpp
	off_t lseek(int fd, off_t offset, int whence);
	```
	Repositions the file offset of an open file descriptor, allowing random access within a file.

24. **fstat**:
	```cpp
	int fstat(int fd, struct stat *buf);
	```
	Retrieves information about an open file, such as its size, permissions, and other attributes.

25. **fcntl**:
	```cpp
	int fcntl(int fd, int cmd, ... /* arg */ );
	```
	Performs various operations on open file descriptors, including setting them to be non-blocking.

26. **poll**:
	```cpp
	int poll(struct pollfd *fds, nfds_t nfds, int timeout);
	```
	Monitors multiple file descriptors for events, providing an efficient way to handle I/O operations with multiple connections.