/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:56:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 11:52:21 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerList.hpp"

ServerList::ServerList(Config &conf) {
	Config::data_type	dataConf = conf.getData();
	
	for (Config::data_type::iterator confIt = dataConf.begin(); confIt != dataConf.end(); confIt++)
	{
		Server server(*confIt);
		_servers.push_back(server);
	}
}

int	ServerList::getNbServers(void) {
	return (_servers.size());
}

void	ServerList::listenConnection(void) {
	serverValue::iterator it = _servers.begin();
	
	while (it != _servers.end()) {
		it->listenConnection();
		it++;
	}
}

ServerList::serverValue::iterator ServerList::getServerbyFd(int fd) {
	serverValue::iterator	it = _servers.begin();
	
	while (it != _servers.end()) {
		if (it->getSockfd() == fd)
			return (it);
		it++;
	}
	return (it);
}

ServerList::serverValue::iterator ServerList::getServerByIpPort(std::vector<std::string> &ipPort) {
	serverValue::iterator	it = _servers.begin();
	
	while (it != _servers.end()) {
		if (it->getIp() == ipPort[0] && it->getPort() == ipPort[1])
			return (it);
		it++;
	}
	return (it);
}
