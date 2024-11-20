/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:17:53 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/20 17:49:38 by aautin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "ft_irc.hpp"

class User
{
	private:
		int				_fd;
		int				_port;
		unsigned int	_size;
		sockaddr_in		_address;
		std::string		_ip;

		std::string 	_nickname;
		std::string 	_name;
		std::string 	_realname;

	public:
		//Constructors-Destructors
		User();
		~User();

		//Setters
		void	beAccepted(int server_fd);
		void	beNamed(std::string &nickname, std::string &name, std::string &realname);

		//Getters
		int		get_fd() const;

		//Exceptions
		class Quit : public std::exception {};
		class Refused : public std::exception {};
};

#endif