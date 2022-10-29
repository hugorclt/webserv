/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/26 15:29:50 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "utils.hpp"
#include "IOpoll.hpp"
#include "Servers.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <cstdlib>
#include <signal.h>

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

ConfigParser::Server	findServ(Request &req, int serverFd, Servers serverList, ConfigParser::data_type conf)
{
	Request::request_type reqData = req.getData();
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

			std::map<int, int>	clientList;
		/* ----------------------------- Server Creation ---------------------------- */
			while (42) 
			{
				try 
				{
					int numberFdReady = epoll_wait(epoll.getEpollfd(), epoll.getEvents(), 1, -1);
					std::cout << numberFdReady << std::endl;
					if (numberFdReady == -1)
						break; //error handle
					for (int i = 0; i < numberFdReady; i++)
					{
						int	clientSocket;
						int	fdTriggered = epoll.getEvents()[i].data.fd;
						Servers::sock_type::iterator sockTarget = serverList.getSocketByFd(fdTriggered);
						if (sockTarget != serverList.getSockIpPort().end())
						{
							clientSocket = serverList.acceptSocket(sockTarget->second);
							epoll.addFd(clientSocket);
							std::cout << "new connection, client: " << clientSocket << " server: " << fdTriggered << std::endl;
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
							int nb_bytes = recv(pairContacted->first, buffer, 1024, 0);
							buffer[nb_bytes] = 0;
							if (nb_bytes == -1)
							{
								std::cerr << "throw error here" << std::endl;
								exit(EXIT_FAILURE);
							}
							else if (nb_bytes)
							{
								std::cout << "new request, client: " << pairContacted->first << " server: " << pairContacted->second << std::endl;
								std::string str(buffer);
								std::cout << str << std::endl; // display request

								Request	req(str);
								ConfigParser::Server server = findServ(req, pairContacted->second, serverList, configServers.getData());
								ConfigParser::Location	env = getEnvFromTarget(req.getTarget(), server);
								Response	res(env, req, serverList.getClientIp(sockTarget->second, pairContacted->first));
								res.execute();
								res.constructData();
								res.sendData(pairContacted->first);
							}
							close(pairContacted->first);
							clientList.erase(pairContacted);
						}
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

					// if (epoll_wait(epoll.getEpollfd(), epoll.getEvents(), 1, -1) != -1)
					// {
					// 	int	newSocket;
					// 	if (sockTarget != serverList.getSockIpPort().end())
					// 	{
					// 		newSocket = serverList.acceptSocket(sockTarget->second);
					// 		epoll.addFd(newSocket);
					// 		std::cout << "new connection, client: " << newSocket << " server: " << clientFd << std::endl;
					// 		clientList.insert(std::make_pair(newSocket, clientFd));
					// 		break;
					// 	}
					// 	else
					// 	{
					// 		std::map<int, int>::iterator clientServer = clientList.find(clientFd);
					// 		if (clientServer == clientList.end())
					// 			break;
					// 		char	buffer[1024] = { 0 };
					// 		int		nb_bytes = recv(clientServer->first, buffer, 1024, 0);
					// 		if (nb_bytes)
					// 		{
					// 			std::cout << "new request, client: " << clientServer->first << " server: " << clientServer->second << std::endl;
					// 			std::string str(buffer);
					// 			//std::cout << str << std::endl; // display request
					// 			Request	req(str);
					// 			ConfigParser::Server server = findServ(req, clientServer->second, serverList, configServers.getData());
					// 			ConfigParser::Location	env = getEnvFromTarget(req.getTarget(), server);
					// 			Response	res(env, req, serverList.getClientIp(sockTarget->second, clientServer->first));
					// 			res.execute();
					// 			res.constructData();
					// 			res.sendData(clientServer->first);
					// 		}
					// 		clientList.erase(clientServer);
					// 		close(newSocket);
					// 		break;
					// 	}
					// }
