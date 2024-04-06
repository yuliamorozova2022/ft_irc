#ifndef __POLLMANAGER__
# define __POLLMANAGER__

#include <poll.h>
#include <iostream>
#include <unistd.h>
#include "./includes.hpp"

#define MAX_CLIENTS 30
#define POLL_TIMEOUT (600 * 1000) //1 min in milliseconds

class PollManager {
	private:
		struct pollfd * _fds;
		int				_max_size;
		int				_cur_size;

	public:
		// Constructors
		PollManager();
		PollManager(int max_clients);
		PollManager(const PollManager &copy);
		PollManager(struct pollfd * fds);
		// Destructor
		~PollManager();
		// Operators
		PollManager & operator=(const PollManager &assign);
		struct pollfd &operator[](unsigned int i) const;

		// Getters / Setters
		struct pollfd   *getFds() const;
		int				getMaxSize() const;
		int				getSize() const;

		// Methods
		void			addFD(int fd);
		void			addFD(int fd, short events);
		void			removeFD(int fd);
		int				poll(int timeout);
};

#endif
