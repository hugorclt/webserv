/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:52:01 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/20 15:26:24 by hrecolet         ###   ########.fr       */
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
		Server(void);
		~Server(void);
		
		int		getSockfd(void) const;
		sockaddr_in		getAddress(void) const;
		void	listenConnection(void);
		int	acceptSocket(void);
};
