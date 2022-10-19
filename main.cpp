/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/19 13:15:19 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "ConfigParser.hpp"
#include "IOpoll.hpp"
#include "Servers.hpp"
#include "HTTPRequest.hpp"
#include "Response.hpp"

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

ConfigParser::Location	getEnvFromTarget(std::string target, ConfigParser::Server serv)
{
	std::vector<std::string>	splitedTarget = split(target, "/");
	ConfigParser::Location		res;
	
	for (std::vector<std::string>::reverse_iterator	rit = splitedTarget.rbegin(); rit != splitedTarget.rend(); rit++)
	{
		std::string	currLoc;
		for (std::vector<std::string>::iterator it = splitedTarget.begin(); std::reverse_iterator(it) != rit; it++)
			currLoc += "/" + *it;
		if (serv.location.count(currLoc))
		{
			res.uniqKey.insert({"_rootToDel_", {currLoc}});
			res.insert(serv.location[currLoc]);
		}
	}
	res.uniqKey.insert({"_rootToDel_", {""}});
	res.insert(serv.location["/"]);
	return (res);
}

ConfigParser::Server	findServ(HTTPRequest &req, int serverFd, Servers serverList, ConfigParser::data_type conf)
{
	HTTPRequest::request_type reqData = req.getData();
	std::string ip = serverList.findIpByFd(serverFd);

	return (selectServ(ip, reqData["Host"][1], reqData["Host"][0], conf));
}

int main(int ac, char **av, char **envSys)
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
									std::cout << str << std::endl;
									HTTPRequest	req(createHttpRequest(str));
									ConfigParser::Server server = findServ(req, serverContacted, serverList, configServers.getData());
									ConfigParser::Location	env = getEnvFromTarget(req.getData()["target"][0], server);
									
									Response	res(env, req, envSys);
									res.execute();
									res.constructData();
									res.sendData(client_fd);
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
