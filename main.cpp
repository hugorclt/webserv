/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/18 13:31:23 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "ConfigParser.hpp"
#include "IOpoll.hpp"
#include "Servers.hpp"
#include "HTTPRequest.hpp"

ConfigParser::Server    selectServ(std::string ip, std::string port, std::string hostName, ConfigParser::data_type vecServs)
{
   ConfigParser::data_type::iterator   firstOccu = vecServs.end();
   for (ConfigParser::data_type::iterator it = vecServs.begin(); it != vecServs.end(); it++)
   {
	   if (!it->listen.count(ip) || !it->listen[ip].count(port))
		   continue ;
		if (it->server_name.count(hostName) || hostName == ip)
			return (*it);
		else if (firstOccu != vecServs.end())
			firstOccu = it;
   }
   return (*firstOccu);
}

ConfigParser::Server	findServ(HTTPRequest &req, int serverFd, Servers serverList, ConfigParser::data_type conf)
{
	HTTPRequest::request_type reqData = req.getData();
	std::string ip = serverList.findIpByFd(serverFd);

	return (selectServ(ip, reqData["Host"][1], reqData["Host"][0], conf));
}

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
					int	serverContacted;
					
					if (epoll_wait(epoll.getEpollfd(), epoll.getEvents(), MAX_EVENTS, -1) != -1)
					{
						int	index = 0;
						int	client_fd = epoll.getEvents()[index].data.fd;
						
						for (index = 0; index < MAX_EVENTS; index++)
						{
							int	newSocket;
							Servers::sock_type::iterator sockTarget = serverList.getSocketByFd(client_fd);
							if (sockTarget != serverList.getSockIpPort().end())
							{
								newSocket = serverList.acceptSocket(sockTarget->second);
								serverContacted = client_fd;
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
									ConfigParser::Server server = findServ(req, serverContacted, serverList, configServers.getData());
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
