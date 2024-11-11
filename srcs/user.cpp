/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:31:27 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/11 16:14:32 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"

User::User(){};
User::User(const User& copy){*this = copy;};

User& User::operator=(const User& copy)
{
	this->_name = copy._name;
	this->_ipAddress = copy._ipAddress;
};

User::~User(){};