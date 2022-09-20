/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:54:36 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/20 15:55:07 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) {
	this->opt  = 1;
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!this->sockfd) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Server was created on fd:" << this->sockfd << std::endl;
	std::cout << "Using ipv4 domain" << std::endl;
	std::cout << "Using sock_stream for tcp connection" << std::endl << std::endl;
	
	
	std::cout << "Setting socket option..." << std::endl;
	if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &this->opt, sizeof(this->opt)) < 0) {
		perror("Socket configuration failed");
		close(this->sockfd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Configuration choosen to reuse address and port" << std::endl << std::endl;
	
	
	std::cout << "Configuration of the address in progress:" << std::endl;
	this->address.sin_family = AF_INET;
	std::cout << "Address uses IPV4" << std::endl;
	this->address.sin_addr.s_addr = INADDR_ANY;
	std::cout << "using INADDR_ANY to bind all interfaces" << std::endl;
	this->address.sin_port = htons(PORT);
	std::cout << "server set on : " << PORT << std::endl << std::endl;
	this->addrLen = sizeof(address);
	
	
	std::cout << "Binding socket to the address..." << std::endl;
	if (bind(this->sockfd, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
		perror("Bind Failed");
		close(this->sockfd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Binding succesful" << std::endl;
	std::cout << "Server initialized" << std::endl << std::endl;
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
	std::cout << "Server listening for connection on port: " << PORT << std::endl;
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
	// int flag = fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
	// std::cout << "socket set on non-blocking" << std::endl;
	// if (flag == -1)
	// {
	// 	close(this->sockfd);
	// 	exit(EXIT_FAILURE);
	// }
	return (newSocket);
}
