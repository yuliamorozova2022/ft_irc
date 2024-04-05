28.03.
TODO:
	- '@' marks opers
		from rfc2811 [https://datatracker.ietf.org/doc/html/rfc2811]:
			Channel operators are identified by the '@' symbol next to their nickname whenever it is associated with 
			a channel (i.e., replies to the NAMES, WHO and WHOIS commands).

	- JOIN channel with '!' prefix is an asspain.
		The creation process for this type of channel is strictly controlled. The user only chooses part of the 
		channel name (known as the channel "short name"), the server automatically prepends the user provided
		name with a channel identifier consisting of five (5) characters. The channel name resulting from the 
		combination of these two elements is unique, making the channel safe from abuses based on network splits.
		The user who creates such a channel automatically becomes "channel creator".  See section 2.4.2 (Channel 
		Creator) for more details on this title.
	- Channels with '+' as prefix do not support channel modes.  This means that all the modes are unset, with 
		the exception of the 't' channel flag which is set.


	- MODE - Change the channel’s mode:

		· i: Set/remove Invite-only channel
				[ 	When the channel flag 'i' is set, new members are only accepted if their mask matches Invite-list 
				(See section 4.3.2) or they have been invited by a channel operator.  This flag also restricts the 
				usage of the INVITE command (See "IRC Client Protocol" [IRC-CLIENT]) to channel operators. ]

		· t: Set/remove the restrictions of the TOPIC command to channel operators
				[ 	The channel flag 't' is used to restrict the usage of the TOPIC command to channel operators. ]

		· k: Set/remove the channel key (password)
				[ 	When a channel key is set (by using the mode 'k'), servers MUST reject their local users request 
				to join the channel unless this key is given. The channel key MUST only be made visible to the 
				channel members in the reply sent by the server to a MODE query. ]

		· o: Give/take channel operator privilege
				[ 	The mode 'o' is used to toggle the operator status of a channel member. ]

		· l: Set/remove the user limit to channel
				[ 	A user limit may be set on channels by using the channel flag 'l'. When the limit is reached, 
				servers MUST forbid their local users to join the channel. The value of the limit MUST only be made 
				available to the channel members in the reply sent by the server to a MODE query. ]

	!!! Note that there is a maximum limit of three (3) changes per command for modes that take a parameter.
		(MODE #Finnish +imI *!*@*.fi) so max length of params is 4 [+ or - and up to 3 params]
done:
	- added _max_lim and _inv_only to the channel object, getters and setters as well
	- added new commands to help() function in commands.cpp
	- beginnnig work on MODE function

02.04

	how weechat works on +channelname and 't' mode and what sends as error?
done:
	- added new member variable in Channel class: bool _t_mode (it indicates if topic can be modified only by opers);
	- added getter/setter for this member var;
	- continuing work on mode

	- Channel names should be case insensitive. In debug I see it is not implemented.
	- Modified join function (added line channel_names[i] = toLower(channel_names[i]); for actually modification of channel name)

03.04
done:
	- MODE +[-]l //maybe can be improved, should be tested more properly
	- MODE +[-]o //maybe can be improved, should be tested more properly
  	- changed getFd() returned value in Client.[h/c]pp : "const Client getFd() const" to "const int getFd() const"
    - changed type of member variable _max_lim in Channel.[h/c]pp : "int _max_lim" to "unsigned int _max_lim".
        When _max_lim is 0 - no limit for members in the channel is set
	- function declarations moved from Server.hpp and mode_command.cpp to includes.hpp

TODO:
    - JOIN has to increment _n_online when client joined some channel
    - JOIN has to check number of clients in channel, if _max_lim is 0 - there is no limit set. If it is, then server will not join 
        client to the channel if this limit is reached(_n_online shows it)
	- MODE +[-]t 
	- MODE +[-]i
	- MODE +[-]k



05.04
	todo:
		toLower() should be modified to handle convertion of next symbols: []\~ to theese: {}|^
		(for channel names or nicknames)