/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:17:14 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/09 15:56:43 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Channel::Channel(): _name("Undefined"), _topic(""), _password(""), _inv_only(false), _restr_topic(false), _limit(-1)
{
};
Channel::Channel(const Channel& copy){*this = copy;};

Channel& Channel::operator=(const Channel& copy)
{
	this->_name = copy._name;
	this->_topic = copy._topic;
	this->_password = copy._password;
	this->_inv_only = copy._inv_only;
	this->_restr_topic = copy._restr_topic;
	this->_limit = copy._limit;
	//copier les vector.
	return *this;
};

Channel::~Channel()
{
	this->_invited_users.clear();
	this->_connected_users.clear();
	//delete et clear les diffeents vector
};

void	Channel::_add_user(User& user)
{
	this->_connected_users.push_back(&user);	
};
void	Channel::_remove_user(User& user)
{
	for (std::vector<User*>::iterator it = this->_connected_users.begin(); it != this->_connected_users.end();)
	{
		if ((**it).get_fd() == user.get_fd())
			this->_connected_users.erase(it);
		it++;
	};
};
