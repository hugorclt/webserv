/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 18:37:09 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int ac, char **av)
{	
	(void)av;
	if (ac > 1 && ac <= 2)
	{		
		try {	
		/* --------------------------------- Parsing -------------------------------- */
			ConfigParser	configServers(av[1]);
			

		// /* ----------------------------- Server Creation ---------------------------- */
		// 	IOpoll	epoll;
		// 	ServerList serverList(configServer);


		// 	serverList.listenConnection();
		// 	epoll.addServerList(serverList);

		// 	while (42) {
		// 		try 
		// 		{
		// 			if (epoll_wait(epoll.getEpollfd(), epoll.getEvents(), MAX_EVENTS, -1) != -1)
		// 			{
		// 				int					index = 0;

		// 				int	client_fd = epoll.getEvents()[index].data.fd;
		// 				for (index = 0; index < MAX_EVENTS; index++)
		// 				{
		// 					ServerList::serverValue::iterator it = serverList.getServerbyFd(client_fd);
		// 					int	newSocket;
		// 					std::cout << (it != serverList.getServers().end());
		// 					if (it != serverList.getServers().end())
		// 					{
		// 						newSocket = (*it)->acceptSocket();
		// 						epoll.addFd(newSocket);
		// 						break;
		// 					}
		// 					else
		// 					{
		// 						char	buffer[1024] = { 0 };
		// 						int		nb_bytes = recv(client_fd, buffer, 1024, 0);
		// 						if (nb_bytes)
		// 						{
		// 							std::string str(buffer);
		// 							std::cout << buffer << std::endl;
		// 							HTTPRequest	req(createHttpRequest(str));
		// 							Response	res(req, *it);
		// 							res.construct();

		// 							res.send(client_fd);
		// 						}
		// 						close(newSocket);
		// 						break;
		// 					}
		// 				}
		// 			}
		// 		} catch (std::exception &e) {
		// 			std::clog << "error: not fatal: server is listening" << std::endl;
		// 			std::cerr << e.what() << std::endl;
		// 		}
		// 	}



		} catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
			return (EXIT_FAILURE);
		}
				
	}
}
