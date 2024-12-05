/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:49:34 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/05 13:44:10 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void disconnectServer(Server &server)
{
	std::vector<pollfd>::iterator pollfds = server.pollfds.begin();
	++pollfds;

	// Loop through all clients to send a disconnect message and close fd
	for (; pollfds < server.pollfds.end(); ++pollfds)
	{

		std::string quit_msg = ":AlKi QUIT :Server shutting down\n";
		send(pollfds->fd, quit_msg.c_str(), quit_msg.length(), 0);
		close(pollfds->fd);
	}

	close(server.pollfds[0].fd);
	server.clients.clear();
	server.pollfds.clear();
}

Server* glob_serv = NULL;

int main(int ac, char **av)
{	
		if (ac != 3)
	{
		std::cerr << "\033[1;31mircserv: invalid arguments.\n\033[3;30mcorrect format is ./ircserv <port> <password>\033[0m" << std::endl;
		return 42;
	}

	int port = atoi(av[1]); // default irc port: 6667
	if (port < 0)
	{
		port = DEFAULT_IRC_PORT;
	}

	manage_signals();
	Server server(port);
	glob_serv = &server; //tempo, juste pour test les signals.

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

						std::cout << "\033[1;31m[SERV]\033[0m new client accepted:" << std::endl;
						std::cout << "\033[1;31m[SERV]\033[0m fd: " << newClient.fd << std::endl;
						std::cout << "\033[1;31m[SERV]\033[0m memoryAdress: " << static_cast<void *>(&newClient.memoryAdress) << std::endl;
						std::cout << "\033[1;31m[SERV]\033[0m size: " << newClient.size << std::endl;
						std::cout << "\033[1;31m[SERV]\033[0m ip: " << newClient.ip << std::endl;
						std::cout << "\033[1;31m[SERV]\033[0m port: " << newClient.port << std::endl
								  << std::endl;

						server.pollfds.push_back(client_pollfd);
						server.clients.push_back(newClient);
						server.add_user(new User(newClient.fd));
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

							std::cout << "\033[1;31m[SERV]\033[0m Client " << server.pollfds[i].fd << " disconnected.\n";

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

	std::cout << "\033[1;31m[SERV]\033[0m Server is closing..." << std::endl;
	disconnectServer(server);
	return EXIT_SUCCESS;
}