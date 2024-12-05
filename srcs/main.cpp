/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:49:34 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/05 17:33:22 by aautin           ###   ########.fr       */
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
		Server server(port, av[1]);

		while (server.get_users_list().size() <= 3)
		{
			int poll_status = server.open_poll();
			if (poll_status <= 0)
				continue;

			for (size_t pollfd_i = 0; pollfd_i < server.get_pollfd().size(); ++pollfd_i) {
				try
				{
					server.handle_poll(pollfd_i);
				}
				catch (Server::UserQuit)
				{
					server.user_quit(pollfd_i);
					std::cout << "User disconnected.\n";
					--pollfd_i;
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
