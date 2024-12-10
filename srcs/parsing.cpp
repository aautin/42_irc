/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:21:27 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/10 17:44:32 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void	parsing(Server& server, int client_fd, Message input)
{
	std::cout << "\033[1;35m[RECV<-" << client_fd << "]\033[0m" << input.get_content() << std::endl;
	
	if (input._command == "CAP")
		cap(client_fd, input._parameters[0]);
	else if (input._command == "NICK")
		nick(server, client_fd, input._parameters[0]);
	else if (input._command == "USER")
		user(server, client_fd, input._parameters[0], input._parameters[2], input._parameters[1]);
	else if (input._command == "PING")
		pong(client_fd, input._parameters[0]);
	else if (input._command == "VERSION")
		version(client_fd);
	else if (input._command == "MOTD")
		motd(server, client_fd);
	else if (input._command == "WHOIS")
		whois(server, client_fd, input._parameters[0]);
	else if (input._command == "PASS")
		pass(server, client_fd, input._parameters[0]);
	else if (input._command == "WHO")
		who(server, client_fd, input._parameters[0]);
	else if (input._command == "JOIN")
	{
		if (input._parameters.size() == 2)
			join(server, client_fd, input._parameters[0], input._parameters[1]);
		else
			join(server, client_fd, input._parameters[0], "");
	}
	else if (input._command == "PART")
	{
		if (input._parameters.size() == 2)
			part(server, client_fd, input._parameters[0], input._parameters[1]);
		else
			part(server, client_fd, input._parameters[0], "");
	}
	else if (input._command == "LIST")
		list(server, client_fd, input._parameters[0]);
	else if (input._command == "PRIVMSG")
		privmsg(server, client_fd, input.get_content());
	else if (input._command == "INVITE")
		invite(server, client_fd, input._parameters[0], input._parameters[1]);
};