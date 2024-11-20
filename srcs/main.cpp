/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:49:34 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/20 17:40:13 by aautin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

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

		while (server.get_users().size() <= 3) {
			int poll_status = server.open_poll();
			if (poll_status <= 0)
				continue;

			for (size_t i = 0; i < server.get_pollfd().size(); ++i) {
				try {
					server.handle_poll(i);
				} catch (User::Quit) {
					server.remove_user(i);
					std::cout << "User disconnected.\n";
					--i;
				} catch (User::Refused) {
					std::perror("accept");
				} catch (Server::System) {
					return EXIT_FAILURE;
				}
			}
		}
	} catch (Server::System const &e) {
		std::perror(e.what());
	}
	
	return EXIT_SUCCESS;
};