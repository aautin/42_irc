/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:49:34 by kpoilly           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/11/26 15:29:00 by kpoilly          ###   ########.fr       */
=======
/*   Updated: 2024/11/20 18:17:21 by aautin           ###   ########.fr       */
>>>>>>> ea44f80a47db521d03b7daad77c078d999d7314b
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

// int main(int ac, char **av)
// {
// 	sockaddr_in clientAddress;
// 	socklen_t clientAddrLen = sizeof(clientAddress);
// 	int client_fd = accept(server.get_servfd(), (sockaddr *)&clientAddress, &clientAddrLen);
// 	if (client_fd < 0)
// 	{
// 		std::perror("accept");
// 		exit(42);
// 	}
// 	std::cout << "\033[1;32mClient is connected.\033[0m" << std::endl;

// 	/*
// 	Test reception de messages

// 	Pour se connecter :
// 	-lancer ./ircserv avec un port et un mdp (pas encore utilise).
// 	-dans un nouveau terminal: "nc 127.0.0.1 port".
// 	-le client est connecte et un message peut etre envoye.
// 	*/

// 	char buffer[100000];
// 	std::string cmd;

// 	while (cmd != "QUIT")
// 	{
// 		ssize_t msg = read(client_fd, buffer, sizeof(buffer) - 1);
// 		if (msg < 0)
// 		{
// 			perror("read");
// 			close(client_fd);
// 			exit(42);
// 		}
// 		buffer[msg] = '\0';
// 		std::cout << "Message: " << buffer << std::endl;
// 		std::istringstream stream(buffer);
// 		while (std::getline(stream, cmd))
// 			parsing(server, cmd);
// 	};

// 	// send(client_fd, "CAP * LS :multi-prefix", 24, 0);
// 	// send(client_fd, "CAP * ACK multi-prefix", 24, 0);

// 	close(client_fd);

// 	return 0;
// };

void disconnectServer(Server &server)
{
	std::vector<pollfd>::iterator pollfds = server.pollfds.begin();
	++pollfds;

	// Loop through all clients to send a disconnect message and close fd
	for (; pollfds < server.pollfds.end(); ++pollfds)
	{

		std::string quit_msg = ":localhost QUIT :Server shutting down\n";
		send(pollfds->fd, quit_msg.c_str(), quit_msg.length(), 0);
		close(pollfds->fd);
	}

	close(server.pollfds[0].fd);
	server.clients.clear();
	server.pollfds.clear();
}

int main(int ac, char **av)
{	
		if (ac != 3)
	{
		std::cerr << "\033[1;31mircserv: invalid arguments.\n\033[3;30mcorrect format is ./ircserv <port> <password>\033[0m" << std::endl;
		return EXIT_SUCCESS;
	}

	int port = atoi(av[1]); // default irc port: 6667
	if (port < 0)
	{
		port = DEFAULT_IRC_PORT;
	}

	Server server(port);

	while (server.clients.size() <= MAX_CLIENTS_NB)
	{
		int ready = poll(server.pollfds.data(), server.pollfds.size(), -1); // 3rd parameter: timeout
		if (ready > 0)
		{
			for (size_t i = 0; i < server.pollfds.size(); ++i)
			{
				if (server.pollfds[i].revents & POLLIN)
				{ // There is something to read
					if (server.pollfds[i].fd == server.fd)
					{ // Read from the server = new client to accept

						Client newClient;
						newClient.size = sizeof(newClient.memoryAdress);
						memset(&newClient.memoryAdress, 0, newClient.size);

						newClient.fd = accept(server.fd, (struct sockaddr *)&newClient.memoryAdress, &newClient.size);
						if (newClient.fd == -1)
							continue;
						pollfd client_pollfd = {newClient.fd, POLLIN, 0};

						newClient.ip = inet_ntoa(newClient.memoryAdress.sin_addr);
						newClient.port = ntohs(newClient.memoryAdress.sin_port);

						std::cout << "Server accepted a new client:" << std::endl;
						std::cout << "fd: " << newClient.fd << std::endl;
						std::cout << "memoryAdress: " << static_cast<void *>(&newClient.memoryAdress) << std::endl;
						std::cout << "size: " << newClient.size << std::endl;
						std::cout << "ip: " << newClient.ip << std::endl;
						std::cout << "port: " << newClient.port << std::endl
								  << std::endl;

						server.pollfds.push_back(client_pollfd);
						server.clients.push_back(newClient);

						// IRC Handshake Messages
						std::string welcome_msg = ":localhost 001 yournickname :Welcome to the IRC Network\r\n";
						std::string host_msg = ":localhost 002 yournickname :Your host is localhost, running version 1.0\r\n";
						std::string created_msg = ":localhost 003 yournickname :This server was created today\r\n";
						std::string motd_msg = ":localhost 372 yournickname :- Welcome to the server MOTD (Message of the Day)\r\n";
						send(newClient.fd, welcome_msg.c_str(), welcome_msg.length(), 0);
						send(newClient.fd, host_msg.c_str(), host_msg.length(), 0);
						send(newClient.fd, created_msg.c_str(), created_msg.length(), 0);
						send(newClient.fd, motd_msg.c_str(), motd_msg.length(), 0);
					}
					else
					{ // Read from a client
						char buffer[1024];
						ssize_t bytes_read = recv(server.pollfds[i].fd, buffer, sizeof(buffer), 0);

						if (bytes_read <= 0)
						{ // Disconnection message
							close(server.pollfds[i].fd);
							server.clients.erase(server.clients.begin() + i - 1); // -1 because 1st client fd is pollfd[1] but 1st client is clients[0]
							server.pollfds.erase(server.pollfds.begin() + i);

							std::cout << "Client disconnected.\n";

							// Adjust index to avoid skipping next fd
							--i;
						}
						else
						{ // Other messages
							buffer[bytes_read] = '\0';
							//std::cout << "\nReceived: " << buffer << std::endl;
							
							std::string cmd;
							std::istringstream stream(buffer);
							while (std::getline(stream, cmd))
 							parsing(server, server.pollfds[i].fd, cmd);
						}
					}
				}
			}
		}
	}

	std::cout << "Server is closing..." << std::endl;
	disconnectServer(server);
	return EXIT_SUCCESS;
}
