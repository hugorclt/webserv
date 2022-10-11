/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:52:01 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 11:51:04 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class Server {		
	private:
		int			_sockfd;
		sockaddr_in	_address;
		int			_opt;
		int			_addrLen;
		std::string	_root;
		std::string _port;
		std::string _ip;

	public:
		Server(Config::map_type serverInfo);
		~Server(void);
		
		int				getSockfd(void) const;
		sockaddr_in		getAddress(void) const;
		void			listenConnection(void);
		int				acceptSocket(void);
		void			sendRequest(int	socket, void *data);
		std::string		getRoot(void) const;
		std::string		getPort(void) const;
		std::string		getIp(void) const ;
};
