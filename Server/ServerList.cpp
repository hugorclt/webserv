/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:56:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 16:27:25 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerList.hpp"

ServerList::ServerList(ConfigParser &confFile) {
	ConfigParser::data_type conf = confFile.getData();
	for (ConfigParser::data_type::iterator it = conf.begin(); it != conf.end(); it++)
	{
		Server *serv = new Server(*it);
		Server::sock_type servSocket = serv->getSocket();
		_sockIpPort.insert(_sockIpPort.end(), servSocket.begin(), servSocket.end());
		_server.push_back(serv);
	}
}

Server::sock_type::iterator ServerList::getSocketByFd(int fd)
{
    Server::sock_type::iterator it = _sockIpPort.begin();
	
    for (; it != _sockIpPort.end(); it++)
    {
        if (it->first.sockfd == fd)
            return (it);
    }
    return (it);
}

int	ServerList::acceptSocket(Server::socket_t sock)
{
	int newSocket = accept(sock.sockfd, (struct sockaddr *)&sock.address, (socklen_t *)&sock.addrLen);
	std::cout << "new connection accepted, new socket: " << newSocket << std::endl;
	if (newSocket < 0)
	{
		perror("Accept failure");
		close(sock.sockfd);
		exit(EXIT_FAILURE);
	}
	int flag = fcntl(sock.sockfd, F_SETFL, O_NONBLOCK);
	std::cout << "socket set on non-blocking" << std::endl;
	if (flag == -1)
	{
		close(sock.sockfd);
		exit(EXIT_FAILURE);
	}
	return (newSocket);
}

ServerList::server_type	ServerList::getServers(void)
{
	return (_server);
}

Server::sock_type	&ServerList::getSockIpPort()
{
	return (_sockIpPort);
}
