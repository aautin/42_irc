/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:21:27 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/13 19:38:20 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void	parsing(Server& server, std::string cmd)
{
	std::cout << "CMD: " << cmd << std::endl;
	std::vector<std::string> split;
	unsigned long start = 0, end = 0;
	while ((start = cmd.find_first_not_of(' ', end)) != std::string::npos) 
	{
		end = cmd.find(' ', start);
		split.push_back(cmd.substr(start, end - start));
	}
	
	if (split[0] == "CAP")
		std::cout << "pouet" << std::endl;
	else if (split[0] == "USER")
		server._user = new User(split[1], split[2], split[3], split[4]);
};