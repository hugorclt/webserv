/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:54:19 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 16:27:28 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Server.hpp"

class Server;

class ConfigParser;

class ServerList {
	public:
		typedef std::vector<Server*>	server_type;
		
	private:
		server_type 		_server;
		Server::sock_type 	_sockIpPort;
		
	public:
		ServerList(ConfigParser &confFile);
		void launchServer(void);

		Server::sock_type::iterator	getSocketByFd(int fd);
		int							acceptSocket(Server::socket_t sock);
		server_type					getServers(void);
		Server::sock_type			&getSockIpPort(void);
};
