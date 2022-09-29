/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/29 15:17:56 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void print_tab(std::vector<std::string> tab)
{
	int	i;

	i = 0;
	while (i < tab.size())
	{
		std::cout << tab[i] << std::endl;
		i++;
	}
}

int main(int ac, char **av)
{	
	(void)av;
	if (ac > 0)
	{
		char	buffer[1024] = { 0 };
		int	index = 0;
		int	newSocket;
		HTTPRequest	req;
		
		//Create poll and server
		IOpoll	epoll;
		Server	server;
		
		// Server listening for connection
		server.listenConnection();
		epoll.addFd(server.getSockfd());
		
		while (42) {
			epoll_wait(epoll.getEpollfd(), epoll.getEvents(), MAX_EVENTS, -1);
			int	client_fd = epoll.getEvents()[index].data.fd;
			for (index = 0; index < MAX_EVENTS; index++)
			{
				if (client_fd == server.getSockfd())
				{
					newSocket = server.acceptSocket();
					epoll.addFd(newSocket);
					break;
				}
				else
				{
					recv(client_fd, buffer, 1024, 0);
					std::string str(buffer);
					std::map<std::string, std::vector<std::string>> mapRequest = createHttpRequest(str);
					req.setData(mapRequest);
					memset(buffer, 0, sizeof(buffer));
					break;
				}
			}
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
