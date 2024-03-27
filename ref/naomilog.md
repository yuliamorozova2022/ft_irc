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
	- Added TOPIC command, which calls RPL topic. doesnt work with multiple channels




TODO:
	- make sure JOIN command works with multiple channels/keys
	- should TOPIC work with multiple channels?
	- we never check if user is actually in the channel before sending message/ requesting topic etc
	- channel modes
	- user modes
	- privmsg! why is it not working