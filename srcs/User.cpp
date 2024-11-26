/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:31:27 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/20 18:18:48 by aautin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

//Constructors-Destructors
User::User() : _size(sizeof(this->_address))
{
	memset(&this->_address, 0, this->_size);
};

User::~User() {};
//------


//Setters
void User::beAccepted(int server_fd)
{
	this->_fd = accept(server_fd, (struct sockaddr*) &this->_address, &this->_size);
	if (_fd >= 0)
	{
		this->_ip = inet_ntoa(this->_address.sin_addr);
		this->_port = ntohs(this->_address.sin_port);

		//To be deleted
		std::cout << "Server accepted a new client:" << std::endl;
		std::cout << "fd: " << this->_fd << std::endl;
		std::cout << "memoryAdress: " << static_cast<void*>(&this->_address) << std::endl;
		std::cout << "size: " << this->_size << std::endl;
		std::cout << "ip: " << this->_ip << std::endl;
		std::cout << "port: " << this->_port << std::endl << std::endl;
		//------
	}
	else
		throw Refused();
};

void User::beNamed(std::string &nickname, std::string &name, std::string &realname)
{
	this->_nickname = nickname;
	this->_name = name;
	this->_realname = realname;
};
//------


//Getters
int User::get_fd() const
{
	return this->_fd;
};
//------
