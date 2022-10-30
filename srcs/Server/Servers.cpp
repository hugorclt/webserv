/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:56:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/30 16:08:53 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"

#include <cstdlib>

void	Servers::_createNewServer(std::string ip, std::string port)
{
	socket_t	socketInfo;
	int			sockfd;
	
	socketInfo.ip = ip;
	socketInfo.port = port;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	socketInfo.sockfd = sockfd;
	if (!sockfd)
		throw ServersError("Socket creation failed");
	socketInfo.opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketInfo.opt, sizeof(socketInfo.opt)) < 0) {
	    close(sockfd);
        throw ServersError("Configuration failed");
	}
	
	socketInfo.address.sin_family = AF_INET;
	socketInfo.address.sin_addr.s_addr = inet_addr(ip.c_str());
	socketInfo.address.sin_port = htons(atoi(port.c_str()));
	socketInfo.addrLen = sizeof(socketInfo.address);
	
	if (bind(sockfd, (struct sockaddr*)&socketInfo.address, sizeof(socketInfo.address)) < 0) {
	    close(sockfd);
		throw ServersError("binding socket failed");
	}
	_sockIpPort.insert(std::make_pair(sockfd, socketInfo));
}

void    Servers::_listenConnection(void)
{
    for (Servers::sock_type::iterator it = _sockIpPort.begin(); it != _sockIpPort.end(); it++)
    {        
        if (listen(it->first, 5) < 0) {
            close(it->first);
            throw ServersError("listen failed");
        }
    }
}

/*
Go through all the server config file to create new server for each pair host/port
*/
Servers::Servers(ConfigParser &confFile) {
	ConfigParser::data_type conf = confFile.getData();
	for (ConfigParser::data_type::iterator itconf = conf.begin(); itconf != conf.end(); itconf++)
	{
		for (ConfigParser::Server::listen_type::iterator itListen = itconf->listen.begin(); itListen != itconf->listen.end(); itListen++)
		{
			for (std::set<std::string>::iterator itSet = itListen->second.begin(); itSet != itListen->second.end(); itSet++)
			{
				_createNewServer(itListen->first, *itSet);
			}
		}
	}
	_listenConnection();
}

Servers::sock_type::iterator Servers::getSocketByFd(int fd)
{
    Servers::sock_type::iterator it = _sockIpPort.begin();
	
    for (; it != _sockIpPort.end(); it++)
    {
        if (it->first == fd)
            return (it);
    }
    return (it);
}

int	Servers::acceptSocket(Servers::socket_t sock)
{
	int newSocket = accept(sock.sockfd, (struct sockaddr *)&sock.address, (socklen_t *)&sock.addrLen);
	if (newSocket < 0)
	{
		close(sock.sockfd);
		throw ServersError("Accept failed");
	}
	int flag = fcntl(newSocket, F_SETFL, O_NONBLOCK);
	if (flag == -1)
	{
		throw ServersError("fcntl failed");
	}
	return (newSocket);
}

Servers::sock_type	&Servers::getSockIpPort()
{
	return (_sockIpPort);
}

std::string	Servers::findIpByFd(int fd)
{
	return (_sockIpPort[fd].ip);
}

std::string Servers::getClientIp(Servers::socket_t &sock, int clientFd)
{
	//WTFFFFFF ?????!!!!! CLEAN THIS SHIT NEVER
	socklen_t len = 0;
	if (getpeername(clientFd, (struct sockaddr *)&(sock.address), &len) == -1)
		throw std::bad_alloc(); // tres adapte NON
	return (std::string(inet_ntoa(sock.address.sin_addr)));
}
