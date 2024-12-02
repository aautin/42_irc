/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:30:58 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/26 15:15:58 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

//Setup
Server::Server(int port)
{
	this->_port = port;
	
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->fd < 0)
	{
		std::perror("socket");
		exit(EXIT_FAILURE);
	}

	this->_address.sin_family = AF_INET;
	this->_address.sin_port = htons(this->_port);
	this->_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->fd, (struct sockaddr*)&this->_address, sizeof(this->_address)))
	{
		std::perror("bind");
		close(this->fd);
		exit(EXIT_FAILURE);

	}
	
	if (listen(this->fd, MAX_CLIENTS_NB) < 0)
	{
		std::perror("listen");
		close(this->fd);
		exit(EXIT_FAILURE);
	}

	std::cout << "\033[1;32mServer is up and listening on port " << port << "\033[0m" << std::endl;

	// Add server fd in the poll of fds
	pollfd serverPollfd = {this->fd, POLLIN, 0};
	this->pollfds.push_back(serverPollfd);

	this->_motd = "Bienvenue sur Discord 2.0!";
};
Server::Server(const Server& copy){*this = copy;};
Server& Server::operator=(const Server& copy)
{
	this->fd = copy.fd;
	this->_port = copy._port;
	this->_address = copy ._address;
	this->_password = copy._password;
	this->_channels_list = copy._channels_list;
	this->_users_list = copy._users_list;
	return *this;
};

Server::~Server()
{
	for (size_t i = 0; i < this->_channels_list.size(); i++)
		delete this->_channels_list[i];
	
	for (size_t i = 0; i < this->_users_list.size(); i++)
		delete this->_users_list[i];

	close(this->fd);
};

//Getters
int						Server::get_servfd()
{
	return this->fd;
};

std::string				Server::get_password()
{
	return this->_password;
};

std::vector<Channel *>	Server::get_channels_list()
{
	return this->_channels_list;
};
std::vector<User *>		Server::get_users_list()
{
	return this->_users_list;
};

User&	Server::get_user(int fd)
{
	for(size_t i = 0; i < this->_users_list.size(); i++)
	{
		if (this->_users_list[i]->get_fd() == fd)
			return *_users_list[i];
	};
	return *_users_list[0];
};

std::string		Server::get_motd()
{
	return this->_motd;
};

//Utils
void	Server::add_channel(Channel *channel)
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