/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:56:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/30 12:03:36 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerList.hpp"

ServerList::ServerList(int nbServer, std::vector<int> portList) {
	this->nbServer = nbServer;
	for (int i = 0; i < nbServer; i++) {
		Server *server = new Server(portList[i]);
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