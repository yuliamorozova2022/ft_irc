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

So basically, the client would send the server basic commands and the server woud respond. basic. server would have to authenticate client, using the password provided in the command line.

irrsi - cli base Irc client.would be good reference
https://irssi.org/download/
