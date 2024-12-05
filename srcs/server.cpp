/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:30:58 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/05 17:29:48 by aautin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

//Setup
Server::Server(int port, std::string const & password)
{
//Open socket on a file descriptor
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
	{
		std::perror("socket()");
		throw std::exception();
	}

	//Configure socket and password
	this->_port = port;
	this->_address.sin_family = AF_INET;
	this->_address.sin_port = htons(this->_port);
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_password = password;
	this->_motd = "Bienvenue sur Discord 2.0!";

	//Add socket options
	this->_socket_options = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR,
		&this->_socket_options,  sizeof(_socket_options)) < 0)
	{
		close(this->_fd);
		std::perror("setsockopt()");
		throw std::exception();
	}

	//Bing socket on a network and a port
	if (bind(this->_fd, (struct sockaddr*)&this->_address, sizeof(this->_address)))
	{
		close(this->_fd);
		std::perror("bind()");
		throw std::exception();
	}

	//Open slots for clients connections
	if (listen(this->_fd, 42) < 0)
	{
		close(this->_fd);
		std::perror("listen()");
		throw std::exception();
	}

	//Add server fd in the pollfd
	pollfd serverPollfd = {this->_fd, POLLIN, 0};
	this->_pollfd.push_back(serverPollfd);

	std::cout << "\033[1;32mServer is up and listening on port " << port << "\033[0m" << std::endl;

	this->_motd = "Bienvenue sur Discord 2.0!";
};

Server::~Server()
{
	for (size_t i = 0; i < this->_users_list.size(); ++i)
		delete this->_users_list[i];
	this->_users_list.clear();

	for (size_t i = 0; i < this->_channels_list.size(); ++i)
		delete this->_channels_list[i];
	this->_channels_list.clear();

	std::vector<pollfd>::iterator pollfd;
	for (pollfd = this->_pollfd.begin(); pollfd < this->_pollfd.end(); ++pollfd)
	{
		std::string quit_msg = ":localhost QUIT :Server shutting down\n";
		send(pollfd->fd, quit_msg.c_str(), quit_msg.length(), 0);
		close(pollfd->fd);
	}
	this->_pollfd.clear();
};

//Getters
int Server::get_servfd()
{
	return this->_fd;
};

int Server::get_port() const
{
	return this->_port;
};

std::string Server::get_password()
{
	return this->_password;
};

std::vector<Channel*> Server::get_channels_list()
{
	return this->_channels_list;
};

std::vector<User*> Server::get_users_list()
{
	return this->_users_list;
};

User& Server::get_user(int fd)
{
	for(size_t i = 0; i < this->_users_list.size(); i++)
	{
		if (this->_users_list[i]->get_fd() == fd)
			return *_users_list[i];
	};
	return *_users_list[0];
};

std::string Server::get_motd()
{
	return this->_motd;
};

std::vector<pollfd> Server::get_pollfd() const
{
	return this->_pollfd;
};

//Utils
int Server::open_poll()
{
	return poll(this->_pollfd.data(), this->_pollfd.size(), -1); // -1: no timeout
}

void Server::handle_poll(int pollfd_i)
{
	if (this->_pollfd[pollfd_i].revents & POLLIN) { // There is something to read
		if (this->_pollfd[pollfd_i].fd == this->_fd) { // Read from server = new user to accept
			unsigned int	size;
			struct sockaddr	address;

			int	fd = accept(this->_fd, &address, &size);
			if (fd < 0)
			{
				std::perror("accept");
				return ;
			}
			this->add_user(new User(fd));
		}
		else { // Read from user
			int user_i = pollfd_i - 1;
			char buffer[1024];
			ssize_t bytes_read = recv(this->_users_list[user_i]->get_fd(), buffer, sizeof(buffer), 0);

			if (bytes_read <= 0) // Disconnection message
				throw UserQuit();
			else // Other messages
			{
				buffer[bytes_read] = '\0';

				std::string new_buffer = _users_list[user_i]->get_buffer() + buffer;
				this->_users_list[user_i]->set_buffer(new_buffer);
				if (new_buffer.find('\n') != std::string::npos
					|| new_buffer.find('\r') != std::string::npos)
				{
					this->communicate(this->_users_list[user_i]);
					this->_users_list[user_i]->set_buffer("");
				}
			}
		}
	}
}

void Server::communicate(User* user)
{
	Message message(user->get_buffer());

	// here, read the user message, apply the command(s) and answer what's needed...
}

void Server::user_quit(int user_index)
{
	int pollfd_index = user_index + 1;

	close(this->_pollfd[pollfd_index].fd);
	this->_pollfd.erase(this->_pollfd.begin() + pollfd_index);

	delete this->_users_list[user_index];
	this->_users_list.erase(this->_users_list.begin() + user_index);
};

void Server::add_channel(Channel *channel)
{
	this->_channels_list.push_back(channel);
};

void	Server::remove_channel(Channel *channel)
{
	for (size_t i = 0; i < this->_channels_list.size(); i++)
	{
		if (this->_channels_list[i] == channel)
		{
			this->_channels_list.erase(this->_channels_list.begin() + i);
			break;
		}
	}
};

void	Server::add_user(User *user)
{
	this->_users_list.push_back(user);
	pollfd user_pollfd = {user->get_fd(), POLLIN, 0};
	this->_pollfd.push_back(user_pollfd);
};

void	Server::remove_user(User *user)
{
	for (size_t i = 0; i < this->_users_list.size(); i++)
	{
		if (this->_users_list[i] == user)
		{
			this->_users_list.erase(this->_users_list.begin() + i);
			break;
		}
	}
};

bool	Server::check_nick(std::string name, int client_fd)
{
	for(size_t i = 0; i < this->_users_list.size(); i++)
	{
		if ((this->_users_list[i]->get_name() == name || (this->_users_list[i]->get_name() + "\r") == name)
			&& client_fd != this->_users_list[i]->get_fd())
			return false;
	}
	return true;
};

void	Server::send_to_all(std::string arg)
{
	for(size_t i = 0; i < this->_users_list.size(); i++)
		stoc(this->_users_list[i]->get_fd(), arg);
};

void	Server::disconnectServer()
{
	std::vector<pollfd>::iterator pollfds = this->_pollfd.begin();
	++pollfds;

	// Loop through all users to send a disconnect message and close fd
	for (; pollfds < this->_pollfd.end(); ++pollfds)
	{
		std::string quit_msg = ":AlKi QUIT :Server shutting down\n";
		send(pollfds->fd, quit_msg.c_str(), quit_msg.length(), 0);
		close(pollfds->fd);
	}

	close(this->_pollfd[0].fd);
	this->_users_list.clear();
	this->_pollfd.clear();
};
