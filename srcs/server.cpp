/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:30:58 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/11 15:49:34 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

//Setup
Server::Server(): _port(0){};
Server::Server(const Server& copy){*this = copy;};

Server& Server::operator=(const Server& copy)
{
	this->_port = copy._port;
	this->_password = copy._password;
	this->_channels_list = copy._channels_list;
	this->_users_list = copy._users_list;
};

Server::~Server()
{
	for (size_t i = 0; i < this->_channels_list.size(); i++)
		delete this->_channels_list[i];
	
	for (size_t i = 0; i < this->_users_list.size(); i++)
		delete this->_users_list[i];
};

//Getters
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