/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:31:27 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/26 16:26:17 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"

User::User(int fd): _clientfd(fd)
{
	this->_name = "";
	this->_ipAddress = "127.0.0.1";
	this->_realname = "";
};
User::User(const User& copy){*this = copy;};

User& User::operator=(const User& copy)
{
	this->_name = copy._name;
	this->_ipAddress = copy._ipAddress;
	this->_realname = copy._realname;
	this->_clientfd = copy._clientfd;
	return *this;
};

User::~User(){};

std::string	User::get_name()
{
	return this->_name;
};

int		User::get_fd()
{
	return this->_clientfd;
};

void		User::set_name(std::string name)
{
	this->_name = name;
	std::cout << "\033[1;31m[SERV]\033[0m user name: " << name << std::endl;
};
void		User::set_IP(std::string IP)
{
	this->_ipAddress = IP;
	std::cout << "\033[1;31m[SERV]\033[0m user IP: " << IP << std::endl;
};
void		User::set_real(std::string real)
{
	this->_realname = real;
	std::cout << "\033[1;31m[SERV]\033[0m user realname: " << real << std::endl;
};
void		User::set_fd(int fd)
{
	this->_clientfd = fd;
	std::cout << "\033[1;31m[SERV]\033[0m user fd: " << fd << std::endl;
};