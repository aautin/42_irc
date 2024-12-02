/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:20:15 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/26 15:16:48 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"
# include "channel.hpp"
# include "user.hpp"
# include "client.hpp"

class Channel;
class User;

class Server
{
	private:
		std::vector<Channel *> 	_channels_list;
		std::vector<User *> 	_users_list;
		
		int						_port;
		sockaddr_in 			_address;
		std::string				_password;
		std::string				_motd;

	public:
		//Setup
		Server(int port);
		Server(const Server& copy);
		Server& operator=(const Server& copy);
		~Server();
	
		//Getters
		int						get_servfd();
		std::string				get_password();
		std::vector<Channel *>	get_channels_list();
		std::vector<User *>		get_users_list();
		User&					get_user(int fd);
		std::string				get_motd();
			
		//Utils
		void	add_channel(Channel *channel);
		void	remove_channel(Channel *channel);
		void	add_user(User *user);
		void	remove_user(User *user);

		void	print_channels();
		void	print_users();

		int						fd;
		std::vector<pollfd>		pollfds;
		std::vector<Client>		clients;
};

#endif