/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aautin <aautin@student.42.fr >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:08:36 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/19 17:26:14 by aautin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <iostream>
# include <sstream>
# include <cstring>
# include <vector>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <poll.h>
# include <csignal>
# include <cstdio>
# include <stdlib.h>
# include <exception>

# include "server.hpp"
# include "channel.hpp"
# include "user.hpp"
# include "utils.hpp"

class Server;

void	parsing(Server& server, std::string cmd);

#endif