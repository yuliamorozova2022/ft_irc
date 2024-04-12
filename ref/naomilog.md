25.03
	- Change channel->addmember to call client->addchannel, so not both need to be called when adding a client to channel, or when creating a new channel.
	- updated join command function to work with this ^^
	-Add channel prefix [#&] to actual channel name, bc apparently different prefix == different channel which i didnt realize.
	- Updated tools/checkAndLowercaseChannelName to force prefix.
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
	- TOPIC command now allows channel operators to change the channel's topic
	- added RPL_NOTOPIC if the topic is unset. removed default_topic

2.04
	- INVITE works
	- modified NAMES to append '@' to mods.
	- working on join - checking invite list if channel is set to invite-only
	- added clause to check if channel is invite_only in JOIN. no testing bc idk how to add flag to channel currently but should be fine
5.04
	- LIST works
	- deleted file 'privmsg_command.cpp'. now all commands are in commands.cpp
	- added color definitions in include.hpp
	- added rule to skip client prefix if is present

8.04
	-kick command, is not same as ban - does not prevent users from immediately joining
	- fixed memory leak caused by QUIT command and get_command loop - when command QUIT is called the function still tried to read info from (freed) client. solved by checking client exists with each loop

11.04
	- added if to actually check if user is in channel before messaging.

TODO:
	- make sure JOIN command works with multiple channels/keys
	- we never check if user is actually in the channel before sending message/ requesting topic etc (or do we?)
	- channel modes
	- user modes (?)
	- file transfers (?)
	- PART command
	- KICK command
