/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:54:19 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/08 17:37:19 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"
#include <vector>
#include "Server.hpp"

class Server;

class ServerList {
	private:
		std::vector<Server*>	server;
		int						nbServer;

	public:
		ServerList(int	nbServer, std::map<int, std::map<std::string, std::string>>	portList);
		
		std::string	getRootOfServ(int port);
		int		&getNbServers(void);
		Server	&operator[](const int index);

		void	listenConnection(void);
		int		isServerFd(int fd);
};
