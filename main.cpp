/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/19 11:22:31 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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
		serverFd = socket(AF_INET, SOCK_STREAM, 0);
		if (!serverFd) {
			perror("socket creation failed");
			exit(EXIT_FAILURE);
		}
		if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
			perror("socket configuration failed");
			exit(EXIT_FAILURE);
		}
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(PORT);

		if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
			perror("Bind failed");
			exit(EXIT_FAILURE);
		}
		if (listen(serverFd, 3) < 0) {
			perror("listen failure");
			exit(EXIT_FAILURE);
		}
		newSocket = accept(serverFd, (struct sockaddr*)&address, (socklen_t *)&addrLen);
		if (newSocket < 0) {
			perror("accept failure");
			exit(EXIT_FAILURE);
		}
		valread = read(newSocket, buffer, 1024);
		std::cout << buffer << std::endl;
		send(newSocket, av[1], std::strlen(av[1]), 0);
		std::cout << "message send" << std::endl;

		close(newSocket);
		shutdown(serverFd, SHUT_RDWR);
	}
}

