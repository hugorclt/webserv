/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:52:01 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 13:11:44 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Config.hpp"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>


class Server {		
	private:
		static std::vector<std::pair<int, std::pair<std::string, std::string>>> 
		Config::Server	_serverInfo;
			
	public:
};
