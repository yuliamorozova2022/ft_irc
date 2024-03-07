#include "PollFDs.hpp"

static struct pollfd * init_fds(int max_size)
{
	struct pollfd *fds = new struct pollfd[max_size];

	for (int i = 0; i < max_size; i++)
	{
		fds[i].fd = -1;
		fds[i].events = POLLIN;
	}
	
	return fds;
}


// Constructors
	PollFDs::PollFDs() :_max_size(MAX_CLIENTS), _cur_size(0)
	{
		_fds = init_fds(_max_size);
		std::cout << "\e[0;33mDefault Constructor called of PollFDs\e[0m" << std::endl;
	}
	PollFDs::PollFDs(int max_size) :_max_size(max_size), _cur_size(0)
	{
		_fds = init_fds(_max_size);
	}

	PollFDs::PollFDs(const PollFDs &copy)
	{
		_fds = copy.getFds();
		std::cout << "\e[0;33mCopy Constructor called of PollFDs\e[0m" << std::endl;
	}

	PollFDs::PollFDs(struct pollfd * fds)
	{
		_fds = fds;
		std::cout << "\e[0;33mFields Constructor called of PollFDs\e[0m" << std::endl;
	}


// Destructor
	PollFDs::~PollFDs()
	{
		delete [] _fds;
		std::cout << "\e[0;31mDestructor called of PollFDs\e[0m" << std::endl;
	}


// Operators
	PollFDs & PollFDs::operator=(const PollFDs &assign)
	{
		_fds = assign.getFds();
		return *this;
	}


// Getters / Setters
	struct pollfd * PollFDs::getFds() const {return _fds;}
	int PollFDs::getMaxSize() const {return _max_size;}
	int PollFDs::getSize() const {return _cur_size;}

//methods
	void	PollFDs::addFD(int fd)
	{
		if (_cur_size < _max_size)
			_fds[_cur_size++].fd = fd;
	}

	void	PollFDs::addFD(int fd, short events)
	{
		if (_cur_size < _max_size)
		{
			_fds[_cur_size].fd = fd;
			_fds[_cur_size].events = events;
			_cur_size++;
		}
	}

	void	PollFDs::removeFD(int fd)
	{
		for (int i = 0; i < _cur_size; i++)
		{
			if (_fds[i].fd == fd)
			{
				close (_fds[i].fd);
				for (int j = i; j < _cur_size; j++)
				{
					_fds[j].fd = _fds[j + 1].fd;
					_fds[j].events = _fds[j + 1].events;
				}
				_cur_size --;
			}
	
		}
		
	}