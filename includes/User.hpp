/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:17:53 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/26 16:48:12 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "ft_irc.hpp"

class User
{
	private:
		std::string _nickname;
		std::string _name;
		std::string _ipAddress;
		std::string _realname;
		int 		_clientfd;

	public:
		User(std::string nick, std::string name, std::string ip, std::string real, int fd);
		User(const User& copy);
		User& operator=(const User& copy);
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