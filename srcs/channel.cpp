/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:17:14 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/09 17:26:03 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Channel::Channel(): _name("Undefined"), _topic(""), _password(""), _inv_only(false), _restr_topic(false), _limit(0)
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
	this->connected_users.clear();
	//delete et clear les diffeents vector
};

void	Channel::_add_user(User& user)
{
	this->connected_users.push_back(&user);	
};
void	Channel::_remove_user(User& user)
{
	for (std::vector<User*>::iterator it = this->connected_users.begin(); it != this->connected_users.end();)
	{
		if ((**it).get_fd() == user.get_fd())
			this->connected_users.erase(it);
		it++;
	};
};

std::string	Channel::get_name()
{
	return this->_name;	
};

std::string	Channel::get_topic()
{
	return this->_topic;	
};

bool	Channel::is_op(User& user)
{
	for (std::vector<User*>::iterator it = this->_op_users.begin(); it != this->_op_users.end();)
	{
		if ((**it).get_real() == user.get_real())
			return true;
		it++;
	};
	return false;
};

bool	Channel::is_invited(User& user)
{
	for (std::vector<std::string>::iterator it = this->_invited_users.begin(); it != this->_invited_users.end();)
	{
		if (*it == user.get_real())
			return true;
		it++;
	};
	return false;
};

void	Channel::join(User &user, std::string password)
{
		//check si channel en inv only et user invited
		if (this->_inv_only && !this->is_invited(user))
		{
			stoc(user.get_fd(), ERR_INVITEONLYCHAN + user.get_name() + " " + this->get_name() + " :Invite Only (+i).\r\n");
			return;
		}
			
		//check si channel est limited et si limite pas atteinte
		if (this->_limit != 0 && this->connected_users.size() >= this->_limit)
		{
			stoc(user.get_fd(), ERR_CHANNELISFULL + user.get_name() + " " + this->get_name() + " :Channel is full (+l).\r\n");
			return;
		}
			
		//Check if password needed or not (if yes, is the right password given)
		if(this->_password.empty() || password == this->_password)
		{
			user.join_channel(*this);
			this->_add_user(user);
			this->send_connected_users(user);
			return;
		}
		else
		{
			stoc(user.get_fd(), ERR_BADCHANNELKEY + user.get_name() + " " + this->get_name() + " :Password mismatch (+k).\r\n");
			return;
		}
};

void	Channel::send_connected_users(User& user)
{
	std::string tosend = "353 " + user.get_name() + "!" + user.get_real() + "@" + user.get_IP() + "=" + this->get_name() + ":";
	for (size_t i = 0; i < this->connected_users.size(); i++)
	{
		if (this->is_op(*(this->connected_users[i])))
			tosend += "@";
		tosend += this->connected_users[i]->get_name() + " ";	
	};
	stoc(user.get_fd(), tosend + "\r\n");
	stoc(user.get_fd(), "366 " + user.get_name() + "!" + user.get_real() + "@" + user.get_IP() + " " + this->get_name() + " :End of /NAMES list\r\n");
	
};

void	Channel::who_cmd(Server& server, int client_fd)
{
	User& user = server.get_user(client_fd);
	
	for (size_t i = 0; i < this->connected_users.size(); i++)
	{
		std::string tosend = "352 " + user.get_name() + "!" + user.get_real() + "@" + user.get_IP() + " " + this->get_name() + " "
		+ this->connected_users[i]->get_name() + " " + this->connected_users[i]->get_IP() + " "
		+ server.get_ip() + " " + this->connected_users[i]->get_name() + "H";

		if (this->is_op(*(this->connected_users[i])))
			tosend += "@";
		stoc(user.get_fd(), tosend + " :0" + this->connected_users[i]->get_real() + "\r\n");		
	};
	stoc(user.get_fd(), "315 " + user.get_name() + "!" + user.get_real() + "@" + user.get_IP() + " :End of /NAMES list\r\n");
	
};
