#include <unistd.h>
#include <iostream>

#include "./PollManager.hpp"

int main(void)
{
	PollManager pfd(33);
	pfd.addFD(3);
	pfd.addFD(5);
	pfd.addFD(3);

	pfd.removeFD(44);
	std::cout	<< "max size: " << pfd.getMaxSize()
				<< "\ncurrent size: " << pfd.getSize()
				<< std::endl;

	return 0;
}