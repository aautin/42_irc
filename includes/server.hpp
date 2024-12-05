/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:20:15 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/05 17:30:06 by aautin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"

class Channel;
class User;

class Server
{
	private:
		int						_fd;
		int						_port;
		int						_socket_options;
		sockaddr_in 			_address;
		std::string				_password;
		std::string				_motd;
		
		std::vector<Channel*> 	_channels_list;
		std::vector<User*> 		_users_list;

		std::vector<pollfd>		_pollfd;

	public:
		//Setup
		Server(int port, std::string const &password = NULL);
		~Server();
	
		//Getters
		int						get_servfd();
		int						get_port() const;
		std::string				get_password();
		std::vector<Channel*>	get_channels_list();
		std::vector<User*>		get_users_list();
		User&					get_user(int fd);
		std::string				get_motd();
		std::vector<pollfd>		get_pollfd() const;

		//Utils
		int		open_poll();
		void	handle_poll(int pollfd_i);
		void	communicate(User* user);
		void	user_quit(int user_index);


		void	add_channel(Channel *channel);
		void	remove_channel(Channel *channel);
		void	add_user(User *user);
		void	remove_user(User *user);
		bool	check_nick(std::string name, int client_fd);

		void	send_to_all(std::string arg);
		void	disconnectServer();

		//Exceptions
		class UserQuit : public std::exception {};

		// void	print_channels();
		// void	print_users();
};

#endif