#include "PollManager.hpp"

static struct pollfd * init_fds(int max_size)
{
	struct pollfd *fds = new struct pollfd[max_size];

	for (int i = 0; i < max_size; i++)
	{
		fds[i].fd = -1;
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}

	return fds;
}


// Constructors
	PollManager::PollManager() :_max_size(MAX_CLIENTS), _cur_size(0)
	{
		_fds = init_fds(MAX_CLIENTS);
		std::cout << "\e[0;33mDefault Constructor called of PollManager\e[0m" << std::endl;
	}
	PollManager::PollManager(int max_size) :_max_size(max_size), _cur_size(0)
	{
		std::cout << "\e[0;33mInt Constructor called of PollManager\e[0m" << std::endl;
		_fds = init_fds(max_size);
	}

	PollManager::PollManager(const PollManager &copy)
	{
		_fds = copy.getFds();
		std::cout << "\e[0;33mCopy Constructor called of PollManager\e[0m" << std::endl;
	}

	PollManager::PollManager(struct pollfd * fds)
	{
		_fds = fds;
		std::cout << "\e[0;33mFields Constructor called of PollManager\e[0m" << std::endl;
	}


// Destructor
	PollManager::~PollManager()
	{
		for (int i = 0; i < _cur_size; i++)
		{
			if (_fds[i].fd != -1)
				close (_fds[i].fd);
		}
		delete [] _fds;
		std::cout << "\e[92mDestructor called of PollManager\e[0m" << std::endl;
	}


// Operators
	PollManager & PollManager::operator=(const PollManager &assign)
	{
		_fds = assign.getFds();
		return *this;
	}
	struct pollfd &PollManager::operator[](unsigned int i) const
	{
		return _fds[i];
	}


// Getters / Setters
	struct pollfd * PollManager::getFds() const {return _fds;}
	int PollManager::getMaxSize() const {return _max_size;}
	int PollManager::getSize() const {return _cur_size;}

//methods
	void	PollManager::addFD(int fd)
	{
		for (int i = 0; i < _cur_size; i++)
			if (_fds[i].fd == fd && _fds[i].events == POLLIN)
				return;
		if (_cur_size < _max_size)
			_fds[_cur_size++].fd = fd;
	}

	void	PollManager::addFD(int fd, short events)
	{
		for (int i = 0; i < _cur_size; i++)
			if (_fds[i].fd == fd && _fds[i].events == events)
				return;
		if (_cur_size < _max_size)
		{
			_fds[_cur_size].fd = fd;
			_fds[_cur_size].events = events;
			_cur_size++;
		}
	}

	void	PollManager::removeFD(int fd)
	{
		for (int i = 0; i < _cur_size; i++)
		{
			if (_fds[i].fd == fd)
			{
				close (_fds[i].fd);
				for (int j = i; j < _cur_size - 1; j++)
				{
					_fds[j].fd = _fds[j + 1].fd;
					_fds[j].events = _fds[j + 1].events;
				}
				_cur_size --;
			}
		}
	}

	int		PollManager::poll(int timeout)
	{
		return (::poll(getFds(), getSize(), timeout));
	}