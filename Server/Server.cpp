/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 13:04:24 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 15:57:20 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::pair<Server::socket_t, std::pair<std::string, std::string>>> Server::_sockIpPort;

Server::Server(ConfigParser::Server serverInfo) : _serverInfo(serverInfo) {
    _launch();
    _listenConnection();
}

void    Server::_launch(void) {
    for (ConfigParser::Server::listen_type::iterator itListen = _serverInfo.listen.begin(); itListen != _serverInfo.listen.end(); itListen++)
    {
        for (std::set<std::string>::iterator itSet = itListen->second.begin(); itSet != itListen->second.end(); itSet++)
        {
            socket_t socketInfo;
            socketInfo.sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (!socketInfo.sockfd) {
                perror("Socket creation failed");
                exit(EXIT_FAILURE);
            }
            
            if (setsockopt(socketInfo.sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketInfo.opt, sizeof(socketInfo.opt)) < 0) {
                perror("Socket configuration failed");
                close(socketInfo.sockfd);
                exit(EXIT_FAILURE);
            }
                
            socketInfo.address.sin_family = AF_INET;
            socketInfo.address.sin_addr.s_addr = inet_addr(itListen->first.c_str());
            socketInfo.address.sin_port = htons(atoi(itSet->c_str()));
            socketInfo.addrLen = sizeof(socketInfo.address);
            
            if (bind(socketInfo.sockfd, (struct sockaddr*)&socketInfo.address, sizeof(socketInfo.address)) < 0) {
                perror("Bind Failed");
                close(socketInfo.sockfd);
                continue;
            }
           _sockIpPort.push_back(std::make_pair(socketInfo, std::make_pair(itListen->first, *itSet)));
        }
    }
}

void    Server::_listenConnection(void) {
    for (Server::sock_type::iterator it = _sockIpPort.begin(); it != _sockIpPort.end(); it++)
    {        
        if (listen(it->first.sockfd, 5) < 0) {
            perror("Listen failure");
            close(it->first.sockfd);
            exit(EXIT_FAILURE);
        }
    }
}

Server::sock_type   &Server::getSocket(void) const
{
    return (_sockIpPort);
}