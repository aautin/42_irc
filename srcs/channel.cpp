/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:17:14 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/11 16:18:17 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/channel.hpp"

Channel::Channel(){};
Channel::Channel(const Channel& copy){*this = copy;};

Channel& Channel::operator=(const Channel& copy)
{
	this->_name = copy._name;
	return *this;
};

Channel::~Channel(){};
