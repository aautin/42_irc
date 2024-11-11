/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:17:53 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/11 16:10:40 by kpoilly          ###   ########.fr       */
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

	public:
		User();
		User(const User& copy);
		User& operator=(const User& copy);
		~User();
};

#endif