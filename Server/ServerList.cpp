/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:56:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/08 17:37:09 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerList.hpp"

ServerList::ServerList(int nbServer, std::map<int, std::map<std::string, std::string>> dataConfig) {
	this->nbServer = nbServer;
	for (int i = 0; i < nbServer; i++) {
		std::vector<std::string> portHost = split(dataConfig[i]["listen"], " ");
		Server *server = new Server(atoi(portHost[0].c_str()), portHost[1], dataConfig[i]["root"]);
		this->server.push_back(server);
	}
}

int	&ServerList::getNbServers(void) {
	return (this->nbServer);
}

Server	&ServerList::operator[](const int index) {
	return (*this->server[index]);
}

void	ServerList::listenConnection(void) {
	for (int i = 0; i < this->nbServer; i++) {\
		this->server[i]->listenConnection();
	}
}

int	ServerList::isServerFd(int fd) {
	for (int i = 0; i < this->nbServer; i++) {
		if (this->server[i]->getSockfd() == fd)
			return (i);
	}
	return (-1);
}

std::string	ServerList::getRootOfServ(int port) {
	for (int i = 0; i < this->nbServer; i++) {
		if (this->server[i]->getPort() == port)
			return (this->server[i]->getRoot());
	}
}
