/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:32:30 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/05 13:45:07 by kpoilly          ###   ########.fr       */
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

//Command: NICK <arg>
void	nick(Server& server, int client_fd, std::string arg)
{
	User& user = server.get_user(client_fd);
	std::string currNick = user.get_name();

	if (!server.check_nick(arg, client_fd))
	{
		stoc(client_fd, ERR_NICKNAMEINUSE + currNick + " " + arg +
			" :Nickname is already in use\r\n");
		return;
	}
	user.set_name(arg);
	server.send_to_all(":" + currNick + " NICK " + user.get_name() + "\r\n");
	//Afficher un truc sur tous les channels
};

//command USER <nickname> <IP Address> <realname>
void	user(Server& server, int client_fd, std::string name, std::string IP, std::string real)
{
	User& user = server.get_user(client_fd);
	
	while (!server.check_nick(name, client_fd))
		name += "_";
	user.set_name(name);
	user.set_IP(IP);
	user.set_real(real);

	std::string welcome_msg = ":AlKi 001 " + user.get_name() + " :Welcome to Discord2.0\r\n";
	std::string host_msg = ":AlKi 002 " + user.get_name() + " :Your host is AlKi, running version 1.0\r\n";
	std::string created_msg = ":AlKi 003 " + user.get_name() + " :This server was created today\r\n";
	std::string motd_msg = ":AlKi 372 " + user.get_name() + " :- Welcome to the server !\r\n";
	send(user.get_fd(), welcome_msg.c_str(), welcome_msg.length(), 0);
	send(user.get_fd(), host_msg.c_str(), host_msg.length(), 0);
	send(user.get_fd(), created_msg.c_str(), created_msg.length(), 0);
	send(user.get_fd(), motd_msg.c_str(), motd_msg.length(), 0);
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
