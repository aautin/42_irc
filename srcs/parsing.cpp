/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:21:27 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/05 14:29:31 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void	parsing(Server& server, int client_fd, std::string cmd)
{
	std::cout << "\033[1;35m[RECV<-" << client_fd << "]\033[0m" << cmd << std::endl;
	std::vector<std::string> split;
	unsigned long start = 0, end = 0;
	while ((start = cmd.find_first_not_of(' ', end)) != std::string::npos) 
	{
		end = cmd.find(' ', start);
		split.push_back(cmd.substr(start, end - start));
	}
	
	if (split[0] == "CAP")
		cap(client_fd, split[1]);
	else if (split[0] == "NICK")
		nick(server, client_fd, split[1]);
	else if (split[0] == "USER")
		user(server, client_fd, split[1], split[3], split[2]);
	else if (split[0] == "PING")
		pong(client_fd, split[1]);
	else if (split[0] == "VERSION")
		version(client_fd);
	else if (split[0] == "MOTD")
		motd(server, client_fd);
	else if (split[0] == "WHOIS")
		whois(server, client_fd, split[1]);
};