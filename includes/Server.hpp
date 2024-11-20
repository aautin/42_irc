/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:20:15 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/20 18:18:18 by aautin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"
# include "User.hpp"

class User;

class Server
{
	private:
		int						_fd;
		int						_port;
		sockaddr_in				_address;
		std::string				_password;

		std::vector<User>		_users;
		std::vector<pollfd>		_pollfd;

	public:
		//Constructors-Destructors
		Server(int port, std::string const &password = NULL);
		~Server();

		//Getters
		int						get_fd() const;
		int						get_port() const;
		sockaddr_in				get_address() const;
		std::string				get_password() const;
		std::vector<User>		get_users() const;
		std::vector<pollfd>		get_pollfd() const;

		//Poll
		int		open_poll();
		void	handle_poll(int index);

		//Setters
		void	remove_user(int index);
};

#endif