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

27.03
	- seems that RPL_TOPIC should be in the format
				[332 <client.nickname> <channel.name> :<channel.topic>]
		In the previous version we skipped the user's nickname which is why weechat displayed the topic message weird.
	- Added TOPIC command, which calls RPL_topic. doesnt work with multiple channels

28.03
	- added server.sendToEveryone message that sends to all connected members. currently used only in NICK
	- fixed nick command function to be able to change nick after user already registered.
	- PRIV_MSG works!!! both to channel and user. AYYYYYYYY



TODO:
	- make sure JOIN command works with multiple channels/keys
	- we never check if user is actually in the channel before sending message/ requesting topic etc
	- TOPIC should be able to change a channel's topic aswell
	- channel modes
	- user modes (?)
