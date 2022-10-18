/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/18 12:21:25 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "ConfigParser.hpp"
#include "IOpoll.hpp"
#include "Servers.hpp"

int main(int ac, char **av)
{	
	(void)av;
	if (ac > 1 && ac <= 2)
	{		
		try {	
		/* --------------------------------- Parsing -------------------------------- */
			ConfigParser	configServers(av[1]);
			Servers serverList(configServers);
			IOpoll	epoll(serverList);

		/* ----------------------------- Server Creation ---------------------------- */
			while (42) {
				try 
				{
					if (epoll_wait(epoll.getEpollfd(), epoll.getEvents(), MAX_EVENTS, -1) != -1)
					{
						int					index = 0;

						int	client_fd = epoll.getEvents()[index].data.fd;
						for (index = 0; index < MAX_EVENTS; index++)
						{
							int	newSocket;
							Servers::sock_type::iterator sockTarget = serverList.getSocketByFd(client_fd);
							if (sockTarget != serverList.getSockIpPort().end())
							{
								newSocket = serverList.acceptSocket(sockTarget->first);
								epoll.addFd(newSocket);
								break;
							}
							else
							{
								char	buffer[1024] = { 0 };
								int		nb_bytes = recv(client_fd, buffer, 1024, 0);
								if (nb_bytes)
								{
									std::string str(buffer);
									HTTPRequest	req(createHttpRequest(str));


									// Response	res(req, *it);
									// res.construct();

									// res.send(client_fd);
								}
								close(newSocket);
								break;
							}
						}
					}
				} catch (std::exception &e) {
					std::clog << "error: not fatal: server is listening" << std::endl;
					std::cerr << e.what() << std::endl;
				}
			}



		} catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
			return (EXIT_FAILURE);
		}
				
	}
}
