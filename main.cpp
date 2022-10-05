/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/05 03:14:37 by hrecolet         ###   ########.fr       */
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

ServerList	createServers(Config configServer) {
	std::map<int, std::map<std::string, std::string>>	dataConfig;
	int	nbServer;

	nbServer = configServer.getNbServers();
	dataConfig = configServer.getData();
	ServerList	server(nbServer, dataConfig);
	return (server);
}

int main(int ac, char **av)
{	
	(void)av;
	if (ac > 1 && ac <= 2)
	{
		std::vector<int>	portList;
		char				buffer[1024] = { 0 };
		int					index = 0;
		int					newSocket;
		HTTPRequest			req;
		
		Config	configServer(av[1]);

		//Create poll and server
		IOpoll	epoll;
		ServerList serverList = createServers(configServer);
		
		// Server listening for connection
		serverList.listenConnection();
		epoll.addServerList(serverList);
		
		while (42) {
			if (epoll_wait(epoll.getEpollfd(), epoll.getEvents(), MAX_EVENTS, -1) != -1)
			{
				int	client_fd = epoll.getEvents()[index].data.fd;
				for (index = 0; index < MAX_EVENTS; index++)
				{
					int serverIndex = serverList.isServerFd(client_fd);
					if (serverIndex >= 0)
					{
						newSocket = serverList[serverIndex].acceptSocket();
						epoll.addFd(newSocket);
						break;
					}
					else
					{
						int	nb_bytes = recv(client_fd, buffer, 1024, 0);
						if (nb_bytes)
						{
							std::string str(buffer);
							std::cout << buffer << std::endl;
							std::map<std::string, std::vector<std::string>> mapRequest = createHttpRequest(str);
							req.setData(mapRequest);
							memset(buffer, 0, sizeof(buffer));
						}
						close(newSocket);
						break;
					}
				}
			}
		}
	}
}
