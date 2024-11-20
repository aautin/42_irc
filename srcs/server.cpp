/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:30:58 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/20 17:53:26 by aautin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

//Constructors-Destructors
Server::Server(int port, std::string const &password)
{
	//Open socket on a file descriptor
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd < 0)
		throw System("Socket opening");

	//Configure socket and password
	this->_port = port;
	this->_address.sin_family = AF_INET;
	this->_address.sin_port = htons(this->_port);
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_password = password;

	//Bing socket on a network and a port
	if (bind(this->_fd, (struct sockaddr*)&this->_address, sizeof(this->_address)))
	{
		close(this->_fd);
		throw System("Binding socket on network and port");
	}

	//Open slots for clients connections
	if (listen(this->_fd, 42) < 0)
	{
		close(this->_fd);
		throw System("Listening for clients connections");
	}

	//Add server fd in the pollfd
	pollfd serverPollfd = {this->_fd, POLLIN, 0};
	this->_pollfd.push_back(serverPollfd);

	std::cout << "\033[1;32mServer is up and listening on port " << port << "\033[0m" << std::endl;
};

Server::~Server()
{
	std::vector<pollfd>::iterator	pollfd = this->_pollfd.begin();

	// Skip server's fd
	++pollfd;

	// Loop through all clients to send a disconnect message and close fd
	for (; pollfd < this->_pollfd.end(); ++pollfd) {

		std::string quit_msg = ":localhost QUIT :Server shutting down\n";
		send(pollfd->fd, quit_msg.c_str(), quit_msg.length(), 0);
		close(pollfd->fd);
	}

	close(this->_pollfd[0].fd);
	this->_users.clear();
	this->_pollfd.clear();
}
//------


//Getters
int Server::get_fd() const
{
	return this->_fd;
};

int Server::get_port() const
{
	return this->_port;
};

sockaddr_in Server::get_address() const
{
	return this->_address;
};

std::string Server::get_password() const
{
	return this->_password;
};

std::vector<Channel> Server::get_channels() const
{
	return this->_channels;
};

std::vector<User> Server::get_users() const
{
	return this->_users;
};

std::vector<pollfd> Server::get_pollfd() const
{
	return this->_pollfd;
};
//------


//Poll
int Server::open_poll()
{
	return poll(this->_pollfd.data(), this->_pollfd.size(), -1); // -1: no timeout
}

void Server::handle_poll(int index)
{
	if (this->_pollfd[index].revents & POLLIN) { // There is something to read
		if (this->_pollfd[index].fd == this->_fd) { // Read from server = new user to accept
			User newUser;
			newUser.beAccepted(this->_fd);
			this->_users.push_back(newUser);

			pollfd new_user_pollfd = {newUser.get_fd(), POLLIN, 0};
			this->_pollfd.push_back(new_user_pollfd);

			// To be deleted : IRC Handshake Messages
			std::string welcome_msg = ":localhost 001 yournickname :Welcome to the IRC Network\r\n";
			std::string host_msg = ":localhost 002 yournickname :Your host is localhost, running version 1.0\r\n";
			std::string created_msg = ":localhost 003 yournickname :This server was created today\r\n";
			std::string motd_msg = ":localhost 372 yournickname :- Welcome to the server MOTD (Message of the Day)\r\n";
			send(newUser.get_fd(), welcome_msg.c_str(), welcome_msg.length(), 0);
			send(newUser.get_fd(), host_msg.c_str(), host_msg.length(), 0);
			send(newUser.get_fd(), created_msg.c_str(), created_msg.length(), 0);
			send(newUser.get_fd(), motd_msg.c_str(), motd_msg.length(), 0);
			//------
		}
		else { // Read from user
			char buffer[1024];
			ssize_t bytes_read = recv(this->_pollfd[index].fd, buffer, sizeof(buffer), 0);

			if (bytes_read <= 0) // Disconnection message
			{
				close(this->_users[index - 1].get_fd());
				throw User::Quit();
			}
			else // Other messages
			{
				buffer[bytes_read] = '\0';
				std::cout << "Received: " << buffer << std::endl;
			}
		}
	}
}
//------


//Setters
void	Server::remove_user(int index)
{
	this->_pollfd.erase(this->_pollfd.begin() + index);
	this->_users.erase(this->_users.begin() + index - 1);
}
// void Server::add_channel(Channel const &channel)
// {
// 	this->_channels.push_back(channel);
// };

// void Server::remove_channel(Channel &channel)
// {
// 	for (size_t i = 0; i < this->_channels.size(); i++)
// 	{
// 		if (this->_channels[i] == channel)
// 		{
// 			this->_channels.erase(this->_channels.begin() + i);
// 			break;
// 		}
// 	}
// };
//------
