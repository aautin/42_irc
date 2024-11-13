/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:30:58 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/13 18:53:12 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

//Setup
Server::Server(int port)
{
	this->_port = port;
	
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
	{
		std::perror("socket");
		exit(42);
	}

	this->_address.sin_family = AF_INET;
	this->_address.sin_port = htons(this->_port);
	this->_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_fd, (struct sockaddr*)&this->_address, sizeof(this->_address)))
	{
		std::perror("bind");
		close(this->_fd);
		exit(42);

	}
	
	if (listen(this->_fd, 42) < 0)
	{
		std::perror("listen");
		close(this->_fd);
		exit(42);
	}

	std::cout << "\033[1;32mServer is up and listening on port " << port << "\033[0m" << std::endl;
};
Server::Server(const Server& copy){*this = copy;};
Server& Server::operator=(const Server& copy)
{
	this->_fd = copy._fd;
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

	close(this->_fd);
};

//Getters
int						Server::get_servfd()
{
	return this->_fd;
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