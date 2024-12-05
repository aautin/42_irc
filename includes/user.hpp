/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:17:53 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/05 14:24:20 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "ft_irc.hpp"

class User
{
	private:
		std::string _name;
		std::string _ipAddress;
		std::string _realname;
		int 		_clientfd;

	public:
		User(int fd);
		User(const User& copy);
		User& operator=(const User& copy);
		~User();

		std::string	get_name();
		std::string	get_IP();
		std::string	get_real();
		int			get_fd();

		void		set_name(std::string name);
		void		set_IP(std::string IP);
		void		set_real(std::string real);
		void		set_fd(int fd);
};

#endif