/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:54:36 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 18:43:48 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(Parsing::map_type serverInfo) 
: _root(serverInfo["root"][0]) , _opt(1) , _port(serverInfo["listen"][0]), _ip(serverInfo["listen"][1]) , _errorPath(serverInfo["error"][0])
{
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!_sockfd) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) < 0) {
		perror("Socket configuration failed");
		close(_sockfd);
		exit(EXIT_FAILURE);
	}
		
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(_ip.c_str());
	_address.sin_port = htons(atoi(_port.c_str()));
	_addrLen = sizeof(_address);
	
	if (bind(_sockfd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
		perror("Bind Failed");
		close(_sockfd);
		exit(EXIT_FAILURE);
	}
}

Server::~Server(void) {
	close(_sockfd);
}

/* -------------------------------------------------------------------------- */
/*                                  Accessor                                  */
/* -------------------------------------------------------------------------- */

int	Server::getSockfd(void) const {
	return (_sockfd);
}

sockaddr_in Server::getAddress(void) const {
	return (_address);
}

std::string	Server::getIp(void) const {
	return (_ip);
}

std::string	Server::getPort(void) const {
	return (_port);
}

std::string	Server::getRoot(void) const {
	return (_root);
}

std::string	Server::getError(void) const {
	return (_errorPath);
}

/* -------------------------------------------------------------------------- */
/*                                   methods                                  */
/* -------------------------------------------------------------------------- */

void	Server::listenConnection(void) {
	//std::cout << _sockfd << std::endl;
	if (listen(_sockfd, 5) < 0) {
		perror("Listen failure");
		close(_sockfd);
		exit(EXIT_FAILURE);
	}
}

int	Server::acceptSocket(void) {
	int newSocket = accept(_sockfd, (struct sockaddr *)&_address, (socklen_t *)&_addrLen);
	std::cout << "new connection accepted, new socket: " << newSocket << std::endl;
	if (newSocket < 0)
	{
		perror("Accept failure");
		close(_sockfd);
		exit(EXIT_FAILURE);
	}
	int flag = fcntl(_sockfd, F_SETFL, O_NONBLOCK);
	std::cout << "socket set on non-blocking" << std::endl;
	if (flag == -1)
	{
		close(_sockfd);
		exit(EXIT_FAILURE);
	}
	return (newSocket);
}
