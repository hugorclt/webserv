/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:52:01 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 16:12:54 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Config.hpp"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Server {		
	private:
		int			_sockfd;
		sockaddr_in	_address;
		int			_opt;
		int			_addrLen;
		std::string	_root;
		std::string _port;
		std::string _ip;
		std::string	_errorPath;

	public:
		Server(std::map<std::string, std::vector<std::string>> serverInfo);
		~Server(void);
		
		void			listenConnection(void);
		int				acceptSocket(void);
		void			sendRequest(int	socket, void *data);

		std::string		getError(void) const;
		int				getSockfd(void) const;
		sockaddr_in		getAddress(void) const;
		std::string		getRoot(void) const;
		std::string		getPort(void) const;
		std::string		getIp(void) const ;
};
