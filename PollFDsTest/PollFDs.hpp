#ifndef POLLFDS_HPP
# define POLLFDS_HPP

# include <iostream>
# include <string>
#include <poll.h>
#include <unistd.h>

#define MAX_CLIENTS 30

class PollFDs
{
	public:
		// Constructors
		PollFDs();
		PollFDs(int max_clients);
		PollFDs(const PollFDs &copy);
		PollFDs(struct pollfd * fds);
		
		// Destructor
		~PollFDs();
		
		// Operators
		PollFDs & operator=(const PollFDs &assign);
		
		// Getters / Setters
		struct pollfd * getFds() const;
		int				getMaxSize() const;
		int				getSize() const;
		

		void			addFD(int fd);
		void			addFD(int fd, short events);
	
		void			removeFD(int fd);

	private:
		struct pollfd * _fds;
		int				_max_size;
		int				_cur_size;
		
};

#endif