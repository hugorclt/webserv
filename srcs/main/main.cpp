/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 12:57:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/10 12:15:16 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "utils.hpp"
#include "IOpoll.hpp"
#include "Servers.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "main.hpp"
#include "colors.hpp"
#include "globalDefine.hpp"
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
		if (it->server_name.count(hostName))
			return (*it);
		else if (firstOccu == vecServs.end())
			firstOccu = it;
   	}
	if (firstOccu == vecServs.end())
		throw InvalidHost("Error: invalid host");
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

ConfigParser::Server	findServ(Request &req, std::string ip, ConfigParser::data_type conf)
{
	Request::request_type reqData = req.getData();
	return (selectServ(ip, reqData["Host"][1], reqData["Host"][0], conf));
}

// leak mem and fd
static void	dirtyExit(const std::string &str) // tmp for test purposes
{
	std::cerr << str << std::endl;
	exit(EXIT_FAILURE);
}

int main(int ac, char **av, char **sysEnv)
{	
	if (ac != 2)
	{
		std::cerr << "Invalid number of arguments for WebServ" << std::endl;
		exit (EXIT_FAILURE);
	}
	try {	
	/* --------------------------------- Parsing -------------------------------- */
		ConfigParser	configServers(av[1]);
		Servers 		serverList(configServers);
		IOpoll			epoll(serverList);

		signal(SIGINT, handle_sig);

		std::map<int, int>			clientList;
		std::map<int, Request>	 	requests;
		std::map<int, Response> 	responses;

	/* ----------------------------- Server loop ---------------------------- */
		std::cout << "init " << C_GREEN << "Success" << C_RESET << ", server is running" << std::endl;
		while (!g_exit) 
		{
			try 
			{
				if (g_exit)
					dirtyExit("SIGINT");
				int numberFdReady = epoll_wait(epoll.getEpollfd(), epoll.getEvents(), QUE_SIZE, -1);
				if (g_exit)
					dirtyExit("SIGINT");
				if (numberFdReady == -1)
					dirtyExit("numberFdReady == -1");
				for (int index = 0; index != numberFdReady; index++)
				{
					int	fdTriggered = epoll.getEvents()[index].data.fd;
					Servers::sock_type::iterator sockTarget = serverList.getSocketByFd(fdTriggered);
					if (sockTarget != serverList.getSockIpPort().end())
					{
						int clientSocket;
						clientSocket = serverList.acceptSocket(sockTarget->second);
						epoll.addFd(clientSocket);
						clientList.insert(std::make_pair(clientSocket, fdTriggered));
						requests.insert(std::make_pair(clientSocket, Request(clientSocket)));
					}
					else
					{
						std::map<int, int>::iterator		pairContacted = clientList.find(fdTriggered);
						std::map<int, Request>::iterator	req = requests.find(fdTriggered);
						int									event = epoll.getEvents()[index].events;

						if (pairContacted == clientList.end())
							dirtyExit("Unknown client");
						if (req == requests.end())
							dirtyExit("Can't find associate request");
						if (event & EPOLLHUP || event & EPOLLERR)
							dirtyExit("EPOLLHUP || EPOLLERR");
						if (event & EPOLLIN)
						{
							// s'il reste à lire on continue. Si la requête est invalide/incomplète recv throw, si la requête est complète on exec le reste du process
							if (!req->second.rec())
								continue ;
							//dirtyExit("TEST");
							ConfigParser::Server	server = findServ(req->second, serverList.findIpByFd(pairContacted->second), configServers.getData());
							ConfigParser::Location	env = getEnvFromTarget(req->second.getTarget(), server);
							responses.insert(std::make_pair(fdTriggered, Response (env, req->second, serverList.getClientIp(sockTarget->second, pairContacted->first), sysEnv)));
							responses[fdTriggered].execute();
							responses[fdTriggered].constructData();
						}
						if (!responses.count(fdTriggered))
							dirtyExit("Can't find asssociate response");
						if (responses.find(fdTriggered)->second.sendData(pairContacted->first))
						{
							close(fdTriggered);
							responses.erase(responses.find(fdTriggered));
							requests.erase(requests.find(fdTriggered));
							clientList.erase(clientList.find(fdTriggered));
						}
						else
							epoll.getEvents()[index].events = EPOLLOUT | EPOLLHUP | EPOLLERR; // set EPOLLOUT au cas où on ait besoin de revenir pour finir l'envoi
					}
				}
			} 
			catch (Request::RequestError &e)
			{
				dirtyExit("Request::RequestError");
				//std::cout << e.what() << std::endl;
			}
			catch (std::exception &e)
			{
				dirtyExit("std::exception");
				//std::cerr << e.what() << std::endl;
				//std::cerr << C_ORANGE << "Server is listening" << C_RESET << std::endl;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "init " << C_RED << "Failed" << C_RESET << std::endl
				  << "error : " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	std::cout << std::endl << C_PURPLE << "GoodBye" << C_RESET << std::endl;
	return (EXIT_SUCCESS);
}
