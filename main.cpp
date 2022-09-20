/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/20 16:40:52 by hrecolet         ###   ########.fr       */
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

int	createEpoll(void) {
	int		epollFd = epoll_create1(0);
	if (epollFd < 0)
	{
		perror("Epoll creation failure");
		exit(EXIT_FAILURE);
	}
	return (epollFd);
}

void	addEpoll(int epoll_fd, int fd, epoll_event *event) {
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, event)) {
		perror("Failed to add fd to epoll list");
		close(epoll_fd);
		exit(EXIT_FAILURE);
	}
}

int	setNonBlocking(int socket, int epoll_fd, int newSocket) {
	int flag = fcntl(socket, F_SETFL, O_NONBLOCK);
	if (flag == -1)
	{
		close(socket);
		close(epoll_fd);
		close(newSocket);
		exit(EXIT_FAILURE);
	}
	return (0);
}

int main(int ac, char **av)
{	
	(void)av;
	if (ac > 0)
	{
		int	newSocket;
		
		//Create poll and server
		IOpoll	epoll;
		Server	server;
		
		// Server listening for connection
		server.listenConnection();
		
		while (1) {
			// Accept the connection and save the socket in newSocket
			newSocket = server.acceptSocket();
			std::cout << "new connection listened" << std::endl;
			
			epoll.addFd(newSocket);
			epoll.waitForchange();
		}
		close(newSocket);
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
