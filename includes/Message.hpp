/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:16:24 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/11 12:21:16 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# define ERR_UNKNOWNCOMMAND	421

# include "ft_irc.hpp"

class Message
{
	private:
		std::string					_prefix;	// optional
		std::vector<std::string>	_parameters;// e.g. message content

	public:
		std::string					_command;	// e.g. JOIN, PRIVMSG, PING
		
		//Constructors-Destructors
		Message(std::string const & message);
		~Message();
		
		//Operator overloads
		Message& operator=(const Message& copy);

		//Utils
		std::string	get_content();
		std::string	get_param(size_t i);
		int			get_nbparam();

		//Exceptions
		class Parsing : public std::exception {
			public:
				Parsing(int type) throw() : _type(type) {}
				~Parsing() throw() {}

				int get_type() const { return _type; }

			private:
				int _type;
		};
};

#endif