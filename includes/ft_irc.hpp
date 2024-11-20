/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:08:36 by kpoilly           #+#    #+#             */
/*   Updated: 2024/11/20 15:04:37 by kpoilly          ###   ########.fr       */
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

# include "server.hpp"
# include "client.hpp"
# include "channel.hpp"
# include "user.hpp"
# include "utils.hpp"

# define DEFAULT_IRC_PORT	6667
# define MAX_CLIENTS_NB		5

class Server;

void	parsing(Server& server, std::string cmd);

#endif