#ifndef POLLFDS_HPP
# define POLLFDS_HPP


#include "./includes.hpp"

#define MAX_CLIENTS 30

class PollManager
{
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
		struct pollfd *	getFds() const;
		int				getMaxSize() const;
		int				getSize() const;

		// Methods
		void			addFD(int fd);
		void			addFD(int fd, short events);

		void			removeFD(int fd);

		int				poll(int timeout);

	private:
		struct pollfd * _fds;
		int				_max_size;
		int				_cur_size;

};


#endif