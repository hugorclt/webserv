/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:52:01 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/30 11:41:37 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class Server {
	private:
		int			sockfd;
		sockaddr_in	address;
		int			opt;
		int			addrLen;

	public:
		Server(int	port);
		~Server(void);
		
		int				getSockfd(void) const;
		sockaddr_in		getAddress(void) const;
		void			listenConnection(void);
		int				acceptSocket(void);
		void			sendRequest(int	socket, void *data);
};
