/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:54:36 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/08 17:38:12 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, std::string ip, std::string root) {
	this->root = root;
	this->port = port;
	this->opt  = 1;
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!this->sockfd) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->opt, sizeof(this->opt)) < 0) {
		perror("Socket configuration failed");
		close(this->sockfd);
		exit(EXIT_FAILURE);
	}
		
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = inet_addr(ip.c_str());
	this->address.sin_port = htons(port);
	std::cout << "Server created will listen on : " << port << std::endl;
	this->addrLen = sizeof(address);
	
	if (bind(this->sockfd, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
		perror("Bind Failed");
		close(this->sockfd);
		exit(EXIT_FAILURE);
	}
}

Server::~Server(void) {
	close(this->sockfd);
}

/* -------------------------------------------------------------------------- */
/*                                  Accessor                                  */
/* -------------------------------------------------------------------------- */

int	Server::getSockfd(void) const {
	return (this->sockfd);
}

sockaddr_in Server::getAddress(void) const {
	return (this->address);
}

/* -------------------------------------------------------------------------- */
/*                                   methods                                  */
/* -------------------------------------------------------------------------- */

void	Server::listenConnection(void) {
	if (listen(this->sockfd, 5) < 0) {
		perror("Listen failure");
		close(this->sockfd);
		exit(EXIT_FAILURE);
	}
}

int	Server::acceptSocket(void) {
	int newSocket = accept(this->sockfd, (struct sockaddr *)&this->address, (socklen_t *)&this->addrLen);
	std::cout << "new connection accepted, new socket: " << newSocket << std::endl;
	if (newSocket < 0)
	{
		perror("Accept failure");
		close(this->sockfd);
		exit(EXIT_FAILURE);
	}
	int flag = fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
	std::cout << "socket set on non-blocking" << std::endl;
	if (flag == -1)
	{
		close(this->sockfd);
		exit(EXIT_FAILURE);
	}
	return (newSocket);
}

std::string	Server::getRoot(void) {
	return (this->root);
}

int	Server::getPort(void) {
	return (this->port);
}
