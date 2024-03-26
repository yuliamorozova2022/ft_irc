25.03
	- Change channel->addmember to call client->addchannel, so not both need to be called when adding a client to channel, or when creating a new channel.
	- updated join command function to work with this ^^
	-Add channel prefix [#&] to actual channel name, bc apparently different prefix == different channel which i didnt realize.
	- Updated tools/getChannelName to force prefix.
26.03
	- Added a static greetClientToChannel function to be used in join - it sends JOIN and NAMES messages.
	- Moved RPL functions to src/replies.cpp
	- Added include.hpp file to include everything at once
	- Privmsg seems to be working on surface, but something is incorrect when trying to actually use it? Also, formatting from wechat PRIVMSG has server name in the privmsg command, is that part of regular formatting?