/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:54:19 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 14:00:59 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include "Server.hpp"
#include "Config.hpp"
#include <string>

class Server;

class Config;

class ServerList {
	public:
		typedef std::vector<Server*>	serverValue;
	private:
		serverValue	_servers;

	public:
		ServerList(Config &conf);
		
		std::string getRootOfServ(int port);
		int			getNbServers(void);

		void					listenConnection(void);
		serverValue::iterator	getServerByIpPort(std::vector<std::string> &ipPort);
		serverValue::iterator 	getServerbyFd(int fd);
		serverValue				&getServers(void);
};
