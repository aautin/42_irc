/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:49:34 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/11 14:21:26 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "\033[1;31mircserv: invalid arguments.\n\033[3;30mcorrect format is ./ircserv <port> <password>\033[0m" << std::endl;
		return 42;
	}
	
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		std::perror("socket");
		return 42;
	}
	
	int port = atoi(av[1]); //default irc port: 6667
	if(port < 0)
	{
		std::perror("port");
		return 42;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)))
	{
		std::perror("bind");
		close(server_fd);
		return 42;
	}
	
	if (listen(server_fd, 5) < 0)
	{
		std::perror("listen");
		close(server_fd);
		return 42;
	}

	std::cout << "\033[1;32mServer is up and listening on port " << port << "\033[0m" << std::endl;
	
	sockaddr_in clientAddress;
	socklen_t clientAddrLen = sizeof(clientAddress);
	int client_fd = accept(server_fd, (sockaddr *)&clientAddress, &clientAddrLen);
	if (client_fd < 0)
	{
		std::perror("accept");
		close(server_fd);
		return 42;
	}
	std::cout << "\033[1;32mClient is connected.\033[0m" << std::endl;

	/*
	Test reception de messages

	Pour se connecter :
	-lancer ./ircserv avec un port et un mdp (pas encore utilise).
	-dans un nouveau terminal: "nc 127.0.0.1 port".
	-le client est connecte et un message peut etre envoye.
	*/

	char buffer[1024];
	ssize_t msg = read(client_fd, buffer, sizeof(buffer)-1);
	if (msg < 0)
	{
		perror("read");
		close(client_fd);
		close(server_fd);
		return 42;
	}
	buffer[msg] = '\0';
	std::cout << "Message: " << buffer << std::endl;
	
	close(client_fd);
	close(server_fd);

	return 0;
};