/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:56:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/09 12:29:12 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerList.hpp"

ServerList::ServerList(int nbServer, std::map<int, std::map<std::string, std::string>> dataConfig) {
	this->nbServer = nbServer;
	for (int i = 0; i < nbServer; i++) {
		std::vector<std::string> portHost = split(dataConfig[i]["listen"], " ");
		Server *server = new Server(atoi(portHost[0].c_str()), portHost[1], dataConfig[i]["root"]);
		this->server.insert(std::make_pair(atoi(portHost[0].c_str()), server));
	}
}

int	&ServerList::getNbServers(void) {
	return (this->nbServer);
}

void	ServerList::listenConnection(void) {
	std::map<int, Server*>::iterator	it = this->server.begin();
	
	while (it != this->server.end()) {
		it->second->listenConnection();
		it++;
	}
}

std::map<int, Server*>::iterator	ServerList::isServerFd(int fd) {
	std::map<int, Server*>::iterator	it = this->server.begin();
	
	while (it != this->server.end()) {
		if (it->second->getSockfd() == fd)
			return (it);
		it++;
	}
	return (it);
}

std::string ServerList::getRootOfServ(int port) {
	std::map<int, Server*>::iterator	it = this->server.begin();
	
	while (it != this->server.end()) {
		if (it->second->getPort() == port)
			return (it->second->getRoot());
		it++;
	}
	return (NULL);
}

std::map<int, Server*>	&ServerList::getServer() {
	return (this->server);
}