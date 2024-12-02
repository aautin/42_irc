/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:32:30 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/26 15:57:57 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

//Command: CAP <arg>
void	cap(int client_fd, std::string arg)
{
	if (arg == "LS")
		stoc(client_fd, "CAP * LS:\r\n");
	else if (arg == "LIST")
		stoc(client_fd, "CAP * LIST:\r\n");
};

//command PING <arg>
void	pong(int client_fd, std::string arg)
{
	stoc(client_fd, "PONG " + arg + "\r\n");	
};

//command VERSION
void	version(int client_fd)
{
	stoc(client_fd, RPL_VERSION);
	stoc(client_fd, RPL_ISUPPORT);
};

//command MOTD
void	motd(Server &server, int client_fd)
{
	if (server.get_motd().empty())
		stoc(client_fd, "422" + server.get_user(client_fd).get_name() + " :No MOTD set\r\n");
	else
		stoc(client_fd, "375" + server.get_user(client_fd).get_name() + " :Message of the Day \r\n372 :" +
		server.get_motd() + "\r\n376 " + server.get_user(client_fd).get_name() + " :End of MOTD.\r\n");
};
