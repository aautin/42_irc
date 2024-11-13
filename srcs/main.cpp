/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:49:34 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/13 19:16:58 by kpoilly          ###   ########.fr       */
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
	
	int port = atoi(av[1]); //default irc port: 6667
	if(port < 0)
	{
		std::perror("port");
		return 42;
	}

	Server server(port);
		
	sockaddr_in clientAddress;
	socklen_t clientAddrLen = sizeof(clientAddress);
	int client_fd = accept(server.get_servfd(), (sockaddr *)&clientAddress, &clientAddrLen);
	if (client_fd < 0)
	{
		std::perror("accept");
		exit(42);
	}
	std::cout << "\033[1;32mClient is connected.\033[0m" << std::endl;

	/*
	Test reception de messages

	Pour se connecter :
	-lancer ./ircserv avec un port et un mdp (pas encore utilise).
	-dans un nouveau terminal: "nc 127.0.0.1 port".
	-le client est connecte et un message peut etre envoye.
	*/
	
	char buffer[100000];
	std::string cmd;
	
	while (cmd != "QUIT")
	{
		ssize_t msg = read(client_fd, buffer, sizeof(buffer)-1);
		if (msg < 0)
		{
			perror("read");
			close(client_fd);
			exit(42);
		}
		buffer[msg] = '\0';
		std::cout << "Message: " << buffer << std::endl;
		std::istringstream stream(buffer);
		while(std::getline(stream, cmd))
		{
			std::cout << "CMD: " << cmd << std::endl;
		};
	};

	//send(client_fd, "CAP * LS :multi-prefix", 24, 0);
	//send(client_fd, "CAP * ACK multi-prefix", 24, 0);
	
	close(client_fd);

	return 0;
};