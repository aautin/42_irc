/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:32:30 by kpoilly           #+#    #+#             */
/*   Updated: 2024/12/10 17:49:55 by kpoilly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

//Command: CAP <arg>
void	cap(int client_fd, std::string arg)
{
	if (arg == "LS")
		stoc(client_fd, "CAP * LS:\r\n");
	else if (arg == "LIST")
		stoc(client_fd, "CAP * LIST:\r\n");
};

//Command: NICK <arg>
void	nick(Server& server, int client_fd, std::string arg)
{
	User& user = server.get_user(client_fd);
	std::string currNick = user.get_name();

	if (!server.check_nick(arg, client_fd))
	{
		stoc(client_fd, ERR_NICKNAMEINUSE + currNick + " " + arg +
			" :Nickname is already in use\r\n");
		return;
	}
	user.set_name(arg);
	server.send_to_all(":" + currNick + " NICK " + user.get_name() + "\r\n");
	for (size_t i = 0; i < user.get_joined().size(); i++)
		user.get_joined()[i]->who_cmd(server, client_fd);
};

//command USER <nickname> <IP Address> <realname>
void	user(Server& server, int client_fd, std::string name, std::string IP, std::string real)
{
	User& user = server.get_user(client_fd);
	
	while (!server.check_nick(name, client_fd))
		name += "_";
	user.set_name(name);
	user.set_IP(IP);
	user.set_real(real);

	std::string welcome_msg = ":AlKi 001 " + user.get_name() + " :Welcome to Discord2.0\r\n";
	std::string host_msg = ":AlKi 002 " + user.get_name() + " :Your host is AlKi, running version 1.0\r\n";
	std::string created_msg = ":AlKi 003 " + user.get_name() + " :This server was created today\r\n";
	std::string motd_msg = ":AlKi 372 " + user.get_name() + " :- Welcome to the server !\r\n";
	send(user.get_fd(), welcome_msg.c_str(), welcome_msg.length(), 0);
	send(user.get_fd(), host_msg.c_str(), host_msg.length(), 0);
	send(user.get_fd(), created_msg.c_str(), created_msg.length(), 0);
	send(user.get_fd(), motd_msg.c_str(), motd_msg.length(), 0);

	std::stringstream str_fd;
	str_fd << client_fd;
	server.send_to_all(":Client" + str_fd.str() + " NICK " + user.get_name() + "\r\n");
};

//command PING <arg>
void	pong(int client_fd, std::string arg)
{
	stoc(client_fd, "PONG " + arg + "\r\n");	
};

//command VERSION
void	version(int client_fd)
{
	stoc(client_fd, RPL_VERSION);
	stoc(client_fd, RPL_ISUPPORT);
};

//command MOTD
void	motd(Server &server, int client_fd)
{
	if (server.get_motd().empty())
		stoc(client_fd, "422" + server.get_user(client_fd).get_name() + " :No MOTD set.\r\n");
	else
		stoc(client_fd, "375" + server.get_user(client_fd).get_name() + " :Message of the Day \r\n372 :" +
		server.get_motd() + "\r\n376 " + server.get_user(client_fd).get_name() + " :End of MOTD.\r\n");
};

//command WHOIS <nicknameorchannel>
void	whois(Server &server, int client_fd, std::string arg)
{
	User& user = server.get_user(client_fd);

	if (arg.empty())
	{
		stoc(client_fd, ERR_NONICKNAMEGIVEN + user.get_name() + " :No nickname given.\r\n");
		return;
	}
	
	if (!server.nick_exists(arg))
	{
		stoc(client_fd, ERR_NOSUCHNICK + user.get_name() + " :Unknown nickame.\r\n");
		return;
	}

	stoc(client_fd, RPL_WHOISUSER + user.get_name() + " " + arg + " " + arg
		 + " " + user.get_IP() + " * :" + user.get_real() + "\r\n");
	std::vector<Channel*> joined = user.get_joined();
	std::string tosend = RPL_WHOISCHANNELS + user.get_name() + " " + arg + " :";
	for (size_t i = 0; i < joined.size(); i++)
	{
		if (joined[i]->is_op(user))
			tosend += "@";
		tosend += joined[i]->get_name() + " ";
	}
	stoc(client_fd, tosend + "\r\n");
	stoc(client_fd, RPL_ENDOFWHOIS + user.get_name() + " " + arg + " :End of /WHOIS list.\r\n");
};
//command PASS <password>
void	pass(Server &server, int client_fd, std::string arg)
{
	User& user = server.get_user(client_fd);
	
	if (server.get_password() != "" && arg != server.get_password() && arg !=server.get_password() + "\r")
	{
		stoc(client_fd, "464 " + user.get_name() + " :Incorrect password.\r\n");
		server.remove_user(user.get_fd());
		//deconnecter le client et son pollfd (les remove des vector et les "desaccepter" ?)
	}
};

//command WHO <nameofachannel>
void	who(Server& server, int client_fd, std::string arg)
{
	server.get_channel(arg).who_cmd(server, client_fd);
}
//command JOIN <channel> <password>
void	join(Server& server, int client_fd, std::string name, std::string password)
{		
		User& user = server.get_user(client_fd);
		if (server.channel_exists(name) && !server.get_channel(name).is_connected(user))
			server.get_channel(name).join(user, password);
		else if (!server.channel_exists(name))
		{
			server.add_channel(name, password);
			server.get_channel(name).join(user, password);
		}

		if (!server.get_channel(name).is_connected(user) || !server.channel_exists(name))
			return ;

		if (server.get_channel(name).get_topic().empty())
			stoc(client_fd, RPL_NOTOPIC + name + " :No topic set for " + name + ".\r\n");
		else
			stoc(client_fd, RPL_TOPIC + user.get_name() + " " + name + " :" + server.get_channel(name).get_topic() + "\r\n");
};

void	part(Server& server, int client_fd, std::string channel, std::string reason)
{
	User& user = server.get_user(client_fd);
	
	if (channel.empty() || !server.get_channel(channel).is_connected(user))
		return;
	
	if(reason.empty())
		reason = user.get_name() + " left " + channel + ".";
	server.get_channel(channel).part(user, reason);
	user.leave_channel(server.get_channel(channel));
};

void	list(Server& server, int client_fd, std::string channel)
{
	User& user = server.get_user(client_fd);
	Channel& tolist = server.get_channel(channel);

	stoc(client_fd, RPL_LIST + user.get_name() + " " + tolist.get_name()
		+ " " + tolist.get_nb_users_str() + " :" + "TOPIC: " + tolist.get_topic() + "\r\n");
	stoc(client_fd, RPL_LISTEND + user.get_name() + " :End of /LIST\r\n");
};

void	privmsg(Server& server, int client_fd, std::string args)
{
	User& user = server.get_user(client_fd);
	if (args.find(":") == std::string::npos)
	{
		stoc(client_fd, ERR_NORECIPIENT + user.get_name() + " :No recipient given.\r\n");
		return;
	}

	std::string	targets = (args.substr(0, args.find(':', 0)));
	targets.erase(0, 9);
	std::string	msg = args.substr(args.find(':'));
	msg.erase(0, 1);

	std::cout << "targets: " << targets << std::endl;
	std::cout << "msg: " << msg << std::endl;

	if (msg.empty())
	{
		stoc(client_fd, ERR_NOTEXTTOSEND + user.get_name() + " :No text to send.\r\n");
		return;
	}

	std::vector<std::string> target;
	unsigned long start = 0, end = 0;
	while ((start = targets.find_first_not_of(',', end)) != std::string::npos) 
	{
		end = targets.find(',', start);
		target.push_back(targets.substr(start, end - start));
	}

	for (size_t i = 0; i < target.size(); i++)
	{
		if (server.nick_exists(target[i]))
			stoc( server.get_user(target[i]).get_fd(), ":" + user.get_name() + "!" + user.get_real() + "@" + user.get_IP() + " PRIVMSG "
			+ target[i] + " :" + msg + "\r\n");
		else if (server.channel_exists(target[i]))
		{
			Channel& channel = server.get_channel(target[i]);
			if (channel.is_connected(user))
				channel.user_to_all(client_fd, ":" + user.get_name() + "!" + user.get_real() + "@" + user.get_IP() + " PRIVMSG "
				+ target[i] + " :" + msg + "\r\n");
			else
				stoc(client_fd, ERR_CANNOTSENDTOCHAN + user.get_name() + " " + channel.get_name() + " :Cannot reach channel.\r\n");
		}
		else
			stoc(client_fd, ERR_NOSUCHNICK + user.get_name() + " " + target[i] + " :No such Nick/Channel.\r\n");
	}
};

void	invite(Server& server, int client_fd, std::string targetname, std::string channelname)
{
	Channel& channel = server.get_channel(channelname);
	User& user = server.get_user(client_fd);
	User& target = server.get_user(targetname);

	if(!channel.is_connected(user))
		stoc(client_fd, ERR_NOTONCHANNEL + user.get_name() + " " + channelname + " :You're not connected on this channel.\r\n");
	else if (channel.is_connected(target))
		stoc(client_fd, ERR_USERONCHANNEL + user.get_name() + " " + targetname + " " + channelname + " :User already on this channel.\r\n");
	else
	{
		stoc(target.get_fd(), ":" + user.get_name() + " INVITE " + targetname + " " + channelname + "\r\n");
		channel.add_invited(targetname);
		stoc(client_fd, RPL_INVITING + user.get_name() + " " + targetname + " " + channelname + "\r\n");
	}
};

//OPs restantes:
//MODE
//QUIT

//+ Channel OPs:
//TOPIC
//NAMES
//KICK