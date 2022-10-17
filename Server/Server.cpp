/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 13:04:24 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 14:37:55 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::pair<int, std::pair<std::string, std::string>>> Server::_sockIpPort;

Server::Server(ConfigParser::Server serverInfo) : _serverInfo(serverInfo) {
    _launch();
}

void    Server::_launch(void) {
    std::cout << (_serverInfo.listen.begin()->first) << std::endl;
    for (ConfigParser::Server::listen_type::iterator itListen = _serverInfo.listen.begin(); itListen != _serverInfo.listen.end(); itListen++)
    {
        std::cout << "cc" << std::endl; 
        for (std::set<std::string>::iterator itSet = itListen->second.begin(); itSet != itListen->second.end(); itSet++)
        {
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (!sock) {
                perror("Socket creation failed");
                exit(EXIT_FAILURE);
            }
            
            if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)) < 0) {
                perror("Socket configuration failed");
                close(sock);
                exit(EXIT_FAILURE);
            }
                
            _address.sin_family = AF_INET;
            _address.sin_addr.s_addr = inet_addr(itListen->first.c_str());
            _address.sin_port = htons(atoi(itSet->c_str()));
            _addrLen = sizeof(_address);
            
            if (bind(sock, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
                perror("Bind Failed");
                close(sock);
                continue;
            }
            std::cout << sock << " " << itListen->first << " " << *itSet << std::endl;
           _sockIpPort.push_back(std::make_pair(sock, std::make_pair(itListen->first, *itSet)));
        }
    }
}