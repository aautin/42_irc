/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:16:24 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/11 14:43:14 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Channel
{
	private:
		std::string _name;

	public:
		Channel();
		Channel(const Channel& copy);
		Channel& operator=(const Channel& copy);
		~Channel();
};

#endif