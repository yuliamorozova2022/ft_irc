#include "./Client.hpp"
#include "./Channel.hpp"


void channel_creator(std::string name, Client &user)
{

}
int main(void)
{
	Client a("Auser", "Anick", 3, "42");
	Client b("Buser", "Bnick", 4, "42");
	Client c("Cuser", "Cnick", 5, "42");


	Channel ch("myUser", a);
	// Channel *ch = new Channel("myUser", a);

	std::cout << ch.getName() << std::endl;
	a.addChannel(&ch);

	std::cout << "yay" << std::endl;

	std::map<std::string, Channel *> temp = a.getJoinedChannels();
	std::cout <<
					temp.find("myUser")->first
	<< std::endl;


// delete ch;
	return 0;
}