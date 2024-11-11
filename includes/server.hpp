/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:20:15 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/11 14:40:37 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"
# include "channel.hpp"
# include "user.hpp"

class Server
{
	private:
		std::vector<Channel *> 	_channels_list;
		std::vector<User *> 	_users_list;
		std::string				password;

	public:
		Server();
		Server(const Server& copy);
		Server& operator=(const Server& copy);
		~Server();

};

#endif