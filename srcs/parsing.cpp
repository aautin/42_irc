/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:21:27 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/10 22:34:23 by aautin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void	parsing(Server& server, int client_fd, Message input)
{
	std::cout << "\033[1;35m[RECV<-" << client_fd << "]\033[0m" << input.get_content() << std::endl;

	std::string					prefix = input.get_prefix();
	std::string					command = input.get_command();
	std::vector<std::string>	parameters = input.get_parameters();

	if (command != "PASS" && !server.get_user(client_fd).is_authenticated())
		stoc(client_fd, "451 " + server.get_user(client_fd).get_name() + " :You have not registered.\r\n");
	else if (command == "CAP")
		cap(client_fd, parameters.at(0));
	else if (command == "NICK")
		nick(server, client_fd, parameters.at(0));
	else if (command == "USER")
		user(server, client_fd, parameters.at(0), parameters.at(2),
			parameters.at(1));
	else if (command == "PING")
		pong(client_fd, parameters.at(0));
	else if (command == "VERSION") // not mandatory
		version(client_fd);
	else if (command == "MOTD") // not mandatory
		motd(server, client_fd);
	else if (command == "WHOIS") // not mandatory
		whois(server, client_fd, parameters.at(0));
	else if (command == "PASS")
		pass(server, client_fd, parameters.at(0));
	else if (command == "WHO") // not mandatory
		who(server, client_fd, parameters.at(0));
	else if (command == "JOIN")
	{
		if (parameters.size() == 2)
			join(server, client_fd, parameters.at(0), parameters.at(1));
		else
			join(server, client_fd, parameters.at(0), "");
	}
	else if (command == "PART")
	{
		if (parameters.size() == 2)
			part(server, client_fd, parameters[0], parameters[1]);
		else
			part(server, client_fd, parameters[0], "");
	}
	else if (command == "LIST") // not mandatory
		list(server, client_fd, parameters[0]);
	else if (command == "QUIT")
	{
		throw Server::UserQuit();
	}
};