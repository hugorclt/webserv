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

		std::map<int, int>	clientList;
		std::vector<char>	request;
		std::map<int, int>::iterator pairContacted;

	/* ----------------------------- Server loop ---------------------------- */
		int	clientSocket = 0;
		std::cout << "init " << C_GREEN << "Success" << C_RESET << ", server is running" << std::endl;
		std::cout << "EPOLLIN: " << EPOLLIN << " EPOLLOUT: " << EPOLLOUT << std::endl;
		while (!g_exit) 
		{
			try 
			{
				if (g_exit)
				{
					if (clientSocket)
						close(clientSocket);
					break ;
				}
				int numberFdReady = epoll_wait(epoll.getEpollfd(), epoll.getEvents(), QUE_SIZE, -1);
				if (g_exit)
				{
					if (clientSocket)
						close(clientSocket);
					break ;
				}
				if (numberFdReady == -1)
				{
					std::cerr << "wrong epoll_wait fd ready number" << std::endl;
					continue ;
				}
				std::cout << "epoll wait triggered, numberFdReady:" << numberFdReady << std::endl;
				for (int index = 0; index != numberFdReady; index++)
				{
					int	fdTriggered = epoll.getEvents()[index].data.fd;
					//std::cout << EPOLLIN << " " << EPOLLOUT << " " << std::endl;
					Servers::sock_type::iterator sockTarget = serverList.getSocketByFd(fdTriggered);
					std::cout << "-------------------" << std::endl;
					std::cout << "Pour fd : " << fdTriggered << std::endl;
					std::cout << (epoll.getEvents()[index].events & EPOLLIN ? "je suis entrain de lire" : "je suis pas entrain de lire") << std::endl;
					std::cout << (epoll.getEvents()[index].events & EPOLLOUT ? "je suis entrain d'ecrire" : "je suis pas entrain d'ecrire") << std::endl;
					if (sockTarget != serverList.getSockIpPort().end())
					{
						std::cout << "Connexion entrante: " << epoll.getEvents()[index].events << std::endl;
						clientSocket = serverList.acceptSocket(sockTarget->second);
						epoll.addFd(clientSocket);
						clientList.insert(std::make_pair(clientSocket, fdTriggered));
					}
					else
					{
						std::cout << "Connexion request: " << (epoll.getEvents()[index].events & EPOLLOUT) << std::endl;
						pairContacted = clientList.find(fdTriggered);
						if (pairContacted == clientList.end())
						{
							std::cerr << "Unregistered client" << std::endl;
							continue ;
						}
						char	buffer[4096];
						int nb_bytes = 1;
						while (epoll.getEvents()[index].events & EPOLLIN)
						{
							nb_bytes = recv(pairContacted->first, &buffer, 4096, 0);
							// std::cout << "bytes_read: " << nb_bytes << std::endl;
							if (nb_bytes == -1)
								epoll.getEvents()[index].events = EPOLLOUT;
							if (nb_bytes > 0)
								request.insert(request.end(), buffer, buffer + nb_bytes);
						}
						// for (size_t i = 0; i < request.size(); i++)
						// 	std::cout << request[i];
						// std::cout << std::endl;
						if (request.empty())
						{
							close(pairContacted->first);
							clientList.erase(pairContacted);
							continue ;
						}
						Request	req(request);
						request.clear();
						epoll.getEvents()[index].events = EPOLLOUT;
						ConfigParser::Server server = findServ(req, serverList.findIpByFd(pairContacted->second), configServers.getData());
						ConfigParser::Location	env = getEnvFromTarget(req.getTarget(), server);
						Response	res(env, req, serverList.getClientIp(sockTarget->second, pairContacted->first), sysEnv);
						res.execute();
						res.constructData();
						if (epoll.getEvents()[index].events & EPOLLOUT)
						{
							res.sendData(pairContacted->first);
							close(pairContacted->first);
							clientList.erase(pairContacted);
							std::cout << "Connexion sortante: " << epoll.getEvents()[index].events << std::endl;
						}
					}
				}
			} 
			catch (Request::RequestError &e) { std::cout << e.what() << std::endl; }
			catch (std::exception &e)
			{
				std::cerr << e.what() << std::endl;
				std::cerr << C_ORANGE << "Server is listening" << C_RESET << std::endl;
				close(pairContacted->first);
				close(clientSocket);
				request.clear();
			}
		}
		if (clientSocket)
			close(clientSocket);
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
