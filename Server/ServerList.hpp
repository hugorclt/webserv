/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:54:19 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 11:53:14 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"
#include <vector>
#include "Server.hpp"

class Server;

class ServerList {
	public:
		typedef std::vector<Server>	serverValue;
	private:
		serverValue	_servers;

	public:
		ServerList(Config &conf);
		
		std::string getRootOfServ(int port);
		int			getNbServers(void);

		void					listenConnection(void);
		serverValue::iterator	getServerByIpPort(std::vector<std::string> &ipPort);
		serverValue::iterator 	getServerbyFd(int fd);

};
