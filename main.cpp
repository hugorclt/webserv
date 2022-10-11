/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 10:48:41 by hrecolet         ###   ########.fr       */
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

	//nbServer = configServer.getNbServers();
	//dataConfig = configServer.getData();
	ServerList	server(nbServer, dataConfig);
	return (server);
}

void	printMap(std::map<std::string, std::vector<std::string>> map)
{
	for (std::map<std::string, std::vector<std::string>>::iterator	it = map.begin(); it != map.end(); it++)
	{
		std::cout << it->first << " : [" << it->second.size() << "]";
		for (std::vector<std::string>::iterator itVec = it->second.begin(); itVec != it->second.end(); itVec++)
		{
			std::cout << " " << *itVec;
		}
		std::cout << std::endl;
	}
}

int main(int ac, char **av)
{	
	(void)av;
	if (ac > 1 && ac <= 2)
	{
		// char				buffer[1024] = { 0 };
		// int					index = 0;
		// int					newSocket;
		
		try {
			try {
				Config	configServer(av[1]);
			} catch (Config::ParsingError &e) {
				std::cerr << e.what() << std::endl;
				return (EXIT_FAILURE);
			}


			// try {
				
			// } catch () {
				
			// }	
		} catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
			return (EXIT_FAILURE);
		}

		//Create poll and server
		// IOpoll	epoll;
		// ServerList serverList = createServers(configServer);
		
		// // Server listening for connection
		// serverList.listenConnection();
		// epoll.addServerList(serverList);
		
		// while (42) {
		// 	if (epoll_wait(epoll.getEpollfd(), epoll.getEvents(), MAX_EVENTS, -1) != -1)
		// 	{
		// 		int	client_fd = epoll.getEvents()[index].data.fd;
		// 		for (index = 0; index < MAX_EVENTS; index++)
		// 		{
		// 			std::map<int, Server*>::iterator it = serverList.isServerFd(client_fd);
		// 			if (it != serverList.getServer().end())
		// 			{
		// 				newSocket = it->second->acceptSocket();
		// 				epoll.addFd(newSocket);
		// 				break;
		// 			}
		// 			else
		// 			{
		// 				int	nb_bytes = recv(client_fd, buffer, 1024, 0);
		// 				if (nb_bytes)
		// 				{
		// 					std::string str(buffer);
		// 					std::cout << buffer << std::endl;
		// 					HTTPRequest		req(createHttpRequest(str));
		// 					HTTPResponse	res(req, serverList);

		// 					res.sendRequest(client_fd);
		// 					memset(buffer, 0, sizeof(buffer));
		// 				}
		// 				close(newSocket);
		// 				break;
		// 			}
		// 		}
		// 	}
		// }
	}
}
