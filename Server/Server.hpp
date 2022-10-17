/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:52:01 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 15:57:04 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "ConfigParser.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>


class Server {
	public:
		struct socket_t {
			int			sockfd;
			sockaddr_in	address;
			int			opt;
			int			addrLen;
		};

		typedef std::vector<std::pair<socket_t, std::pair<std::string, std::string>>> sock_type;
		
	private:
		static sock_type 		_sockIpPort;
		ConfigParser::Server	_serverInfo;

		
	public:
		Server(ConfigParser::Server serverInfo);
		

		void		_launch(void);
		void    	_listenConnection(void);
		sock_type	&getSocket(void) const;

};
