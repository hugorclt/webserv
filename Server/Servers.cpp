/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:56:41 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 17:01:00 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"

void	Servers::_createNewServer(std::string ip, std::string port)
{
	socket_t socketInfo;
	
	socketInfo.sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!socketInfo.sockfd) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(socketInfo.sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketInfo.opt, sizeof(socketInfo.opt)) < 0) {
	    perror("Socket configuration failed");
	    close(socketInfo.sockfd);
	    exit(EXIT_FAILURE);
	}
	
	socketInfo.address.sin_family = AF_INET;
	socketInfo.address.sin_addr.s_addr = inet_addr(ip.c_str());
	socketInfo.address.sin_port = htons(atoi(port.c_str()));
	socketInfo.addrLen = sizeof(socketInfo.address);
	
	if (bind(socketInfo.sockfd, (struct sockaddr*)&socketInfo.address, sizeof(socketInfo.address)) < 0) {
	    perror("Bind Failed");
	    close(socketInfo.sockfd);
	    return ;
	}
	_sockIpPort.push_back(std::make_pair(socketInfo, std::make_pair(ip, port)));
}

void    Servers::_listenConnection(void)
{
    for (Servers::sock_type::iterator it = _sockIpPort.begin(); it != _sockIpPort.end(); it++)
    {        
        if (listen(it->first.sockfd, 5) < 0) {
            perror("Listen failure");
            close(it->first.sockfd);
            exit(EXIT_FAILURE);
        }
    }
}

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
        if (it->first.sockfd == fd)
            return (it);
    }
    return (it);
}

int	Servers::acceptSocket(Servers::socket_t sock)
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

Servers::sock_type	&Servers::getSockIpPort()
{
	return (_sockIpPort);
}
