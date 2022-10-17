/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:52:01 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 14:33:00 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "ConfigParser.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


class Server {		
	private:
		static std::vector<std::pair<int, std::pair<std::string, std::string>>> _sockIpPort;
		ConfigParser::Server	_serverInfo;
		sockaddr_in	_address;
		int			_opt;
		int			_addrLen;
		
	public:
		Server(ConfigParser::Server serverInfo);
		

		void	_launch(void);
};
