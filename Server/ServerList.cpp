/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:56:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 18:47:58 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerList.hpp"

ServerList::ServerList(ConfigParser &conf) {
	ConfigParser::data_type	dataConf = conf.getData();
	
	for (ConfigParser::data_type::iterator confIt = dataConf.begin(); confIt != dataConf.end(); confIt++)
	{
		//Server *server = new Server(*confIt);
		//_servers.push_back(server);
	}
}

int	ServerList::getNbServers(void) {
	return (_servers.size());
}

void	ServerList::listenConnection(void) {
	serverValue::iterator it = _servers.begin();
	
	while (it != _servers.end()) {
		(*it)->listenConnection();
		it++;
	}
}

ServerList::serverValue::iterator ServerList::getServerbyFd(int fd) {
	serverValue::iterator	it = _servers.begin();
	
	while (it != _servers.end()) {
		if ((*it)->getSockfd() == fd)
			return (it);
		it++;
	}
	return (it);
}

ServerList::serverValue::iterator ServerList::getServerByIpPort(std::vector<std::string> &ipPort) {
	serverValue::iterator	it = _servers.begin();
	
	while (it != _servers.end()) {
		if ((*it)->getIp() == ipPort[1] && (*it)->getPort() == ipPort[0])
			return (it);
		it++;
	}
	return (it);
}

ServerList::serverValue	&ServerList::getServers(void) {
	return (_servers);
}
