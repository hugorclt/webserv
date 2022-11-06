/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/06 15:16:11 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "utils.hpp"
#include "IOpoll.hpp"
#include "Servers.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "main.hpp"
#include <cstdlib>
#include <csignal>
#include <cstring>

bool g_exit = false;

ConfigParser::Server    selectServ(std::string ip, std::string port, std::string hostName, ConfigParser::data_type vecServs)
{
   	ConfigParser::data_type::iterator   firstOccu = vecServs.end();
	
   	for (ConfigParser::data_type::iterator it = vecServs.begin(); it != vecServs.end(); it++)
   	{
		if (!it->listen.count(ip) || !it->listen[ip].count(port))
		   continue ;
		if (it->server_name.count(hostName) || hostName == ip)
			return (*it);
		else if (firstOccu == vecServs.end())
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
		for (std::vector<std::string>::iterator it = splitedTarget.begin(); std::reverse_iterator< std::vector<std::string>::iterator >(it) != rit; it++)
			currLoc += "/" + *it;
		if (serv.location.count(currLoc))
		{
			res.uniqKey.insert(std::make_pair("_rootToDel_", std::vector<std::string> (1, currLoc)));
			res.insert(serv.location[currLoc]);
		}
	}
	res.uniqKey.insert(std::make_pair("_rootToDel_", std::vector<std::string> (1, "")));
	res.insert(serv.location["/"]);
	return (res);
}

void	handle_sig(int sig)
{
	if (sig == SIGINT)
		g_exit = true;
}

ConfigParser::Server	findServ(Request &req, int serverFd, Servers serverList, ConfigParser::data_type conf)
{
	Request::request_type reqData = req.getData();
	std::string ip = serverList.findIpByFd(serverFd);

	return (selectServ(ip, reqData["Host"][1], reqData["Host"][0], conf));
}

int main(int ac, char **av, char **sysEnv)
{	
	(void)av;
	if (ac > 1 && ac <= 2)
	{		
		try {	
		/* --------------------------------- Parsing -------------------------------- */
			ConfigParser	configServers(av[1]);
			Servers serverList(configServers);
			IOpoll	epoll(serverList);

			signal(SIGINT, handle_sig);

			std::map<int, int>	clientList;
			std::vector<char>	request;

		/* ----------------------------- Server loop ---------------------------- */
			while (!g_exit) 
			{
				try 
				{
					int numberFdReady = epoll_wait(epoll.getEpollfd(), epoll.getEvents(), 1, -1);
					if (g_exit)
						return (0);
					if (numberFdReady == -1)
						throw std::bad_alloc();
					int	clientSocket;
					int	fdTriggered = epoll.getEvents()[0].data.fd;
					Servers::sock_type::iterator sockTarget = serverList.getSocketByFd(fdTriggered);
					if (sockTarget != serverList.getSockIpPort().end())
					{
						clientSocket = serverList.acceptSocket(sockTarget->second);
						epoll.addFd(clientSocket);
						clientList.insert(std::make_pair(clientSocket, fdTriggered));
					}
					else
					{
						std::map<int, int>::iterator pairContacted = clientList.find(fdTriggered);
						if (pairContacted == clientList.end())
						{
							std::cerr << "error pair not found" << std::endl;
							continue ;
						}
						char	buffer[1024];
						int nb_bytes = 1;
						while (nb_bytes > 0)
						{
							
							memset(buffer, 0, sizeof(buffer));
							nb_bytes = recv(pairContacted->first, &buffer, 1024, 0);
							if (nb_bytes > 0)
								request.insert(request.end(), &buffer[0], &buffer[nb_bytes]);
						}
						for (std::vector<char>::size_type i = 0; i < request.size(); i++)
							std::cout << request[i];
						std::cout << std::endl;
						if (request.empty())
						{
							close(pairContacted->first);
							clientList.erase(pairContacted);
							continue ;
						}
						Request	req(request);
						request.clear();
						ConfigParser::Server server = findServ(req, pairContacted->second, serverList, configServers.getData());
						ConfigParser::Location	env = getEnvFromTarget(req.getTarget(), server);
						Response	res(env, req, serverList.getClientIp(sockTarget->second, pairContacted->first), sysEnv);
						res.execute();
						res.constructData();
						res.sendData(pairContacted->first);
						close(pairContacted->first);
						clientList.erase(pairContacted);
					}
				} 
				catch (std::exception &e)
				{
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
