/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:20:15 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/20 15:15:48 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"
# include "channel.hpp"
# include "user.hpp"

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
			
		//Utils
		void	add_channel(Channel *channel);
		void	remove_channel(Channel *channel);

		User*					_user;
		int						fd;
		std::vector<pollfd>		pollfds;
		std::vector<Client>		clients;
};

#endif