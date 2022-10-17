/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 11:54:19 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 13:07:06 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include "Server.hpp"
#include "Config.hpp"
#include <string>

class ServerList {
	private:
		std::vector<Server*> _server;
	public:
		launchServer(void);	
};