/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:16:24 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/09 15:24:01 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class User;
class Server;

typedef enum modes
{
	INV,	//i	
	TOPIC,	//t
	KEY,	//k
	OP,		//o
	LIMIT	//l
} modes;

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::string _password;

		bool		_inv_only;
		bool		_restr_topic;
		size_t		_limit;
		
		std::vector<std::string> _invited_users;

	public:
		std::vector<User*> _connected_users;

		Channel();
		Channel(const Channel& copy);
		Channel& operator=(const Channel& copy);
		~Channel();
};

#endif