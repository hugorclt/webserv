/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/19 12:40:14 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	createSocket(void) {
	int serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (!serverFd) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	return (serverFd);
}

void	setSocketOption(int serverFd, int *opt) {
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt)) < 0) {
		perror("Socket configuration failed");
		close(serverFd);
		exit(EXIT_FAILURE);
	}
}

void	bindSocket(int serverFd, sockaddr_in *address) {
	if (bind(serverFd, (struct sockaddr*)address, sizeof(*address)) < 0) {
		perror("Bind Failed");
		close(serverFd);
		exit(EXIT_FAILURE);
	}
}

void	listenConnection(int serverFd) {
	if (listen(serverFd, 3) < 0) {
		perror("Listen failure");
		close(serverFd);
		exit(EXIT_FAILURE);
	}
}

int	acceptSocket(int serverFd, sockaddr_in *address, int *addrLen) {
	int newSocket = accept(serverFd, (struct sockaddr *)address, (socklen_t *)addrLen);
	if (newSocket < 0)
	{
		perror("Accept failure");
		close(serverFd);
		exit(EXIT_FAILURE);
	}
	return (newSocket);
}

int main(int ac, char **av)
{	
	if (ac > 0)
	{
		int	serverFd;
		int	newSocket;
		int	valread;
		sockaddr_in	address;
		int		opt = 1;
		char	buffer[1024] = { 0 };
		int		addrLen = sizeof(address);

		// Creating socket file descriptor
		serverFd = createSocket();
		
		// Set option for the newly socket
		setSocketOption(serverFd, &opt);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(PORT);

		// Bin the socket to the port choosen
		bindSocket(serverFd, &address);

		// Listen for new connection on the serverFd
		listenConnection(serverFd);
		
		// Accept the connection and save the socket in newSocket
		newSocket = acceptSocket(serverFd, &address, &addrLen);

		// Write the message received
		valread = read(newSocket, buffer, 1024);
		std::cout << buffer << std::endl;

		// Send confirmation message
		send(newSocket, av[1], std::strlen(av[1]), 0);
		std::cout << "message send" << std::endl;

		close(newSocket);
		close(serverFd);
	}
}

/*		Http request exemple
std::string requestExample("GET /hello.htm HTTP/1.1\r\n
								User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n
								Host: www.tutorialspoint.com\r\n
								Accept-Language: en-us\r\n
								Accept-Encoding: gzip, deflate\r\n
								Connection: Keep-Alive\r\n");
*/
