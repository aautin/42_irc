/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:29:34 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/26 15:58:26 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void	stoc(int client_fd, std::string msg)
{
	std::cout << "\033[1;32mServ to Client : " << msg << "\033[0m" << std::endl;
	send(client_fd, msg.c_str(), msg.size(), 0);
};