/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:49:34 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/09 15:17:39 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Server* glob_serv = NULL;

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "\033[1;31mircserv: invalid arguments.\n\033[3;30mcorrect format is ./ircserv <port> <password>\033[0m" << std::endl;
		return EXIT_SUCCESS;
	}

	int port = atoi(av[1]); //default irc port: 6667
	if(port < 0)
	{
		std::cerr << "\033[1;31mPort must be strictly positive\033[0m" << std::endl;
		return EXIT_SUCCESS;
	}
	
	try {
		manage_signals();
		Server server(port, av[1]);
		glob_serv = &server;

		while (server.get_users_list().size() <= 3)
		{
			int poll_status = server.open_poll();
			if (poll_status <= 0)
				continue;

			std::vector<pollfd> pollfd_list = server.get_pollfd_list();
			std::vector<pollfd>::iterator it = pollfd_list.begin();
			std::vector<pollfd>::iterator end = pollfd_list.end();
			for (; it < end; ++it) {
				try
				{
					std::cout << "handle poll" << std::endl;
					server.handle_poll(*it);
				}
				catch (Server::UserQuit)
				{
					server.user_quit(*it);
					std::cout << "User disconnected.\n";
					--it;
				}
				catch (std::exception)
				{
					return EXIT_FAILURE;
				}
			}
		}
	} catch (std::exception) {}

	return EXIT_SUCCESS;
};
