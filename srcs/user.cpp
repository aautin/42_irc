/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:31:27 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/13 19:39:49 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"

User::User(std::string nick, std::string name, std::string ip, std::string real)
: _nickname(nick), _name(name), _ipAddress(ip), _realname(real)
{
	std::cout << "A new user has been created." << std::endl;
	std::cout << "nickname: " << this->_nickname << std::endl;
	std::cout << "name: " << this->_name << std::endl;
	std::cout << "ip address: " << this->_ipAddress << std::endl;
	std::cout << "real name: " << this->_realname << std::endl;
};
User::User(const User& copy){*this = copy;};

User& User::operator=(const User& copy)
{
	this->_nickname = copy._nickname;
	this->_name = copy._name;
	this->_ipAddress = copy._ipAddress;
	this->_realname = copy._realname;
	return *this;
};

User::~User(){};