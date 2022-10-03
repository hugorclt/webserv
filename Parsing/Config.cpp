/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/03 15:01:20 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(char *fileName) {
	std::ifstream			configFile;
	std::ifstream			serverFile;

	configFile.open(fileName, std::ios::out);
	this->nbServer = countLenServer(configFile);
	serverFile.open(fileName, std::ios::out);
	ft_parse_server(serverFile, this->configFile);
}

std::map<int, std::map<std::string, std::string>>	&Config::getData(void) {
	return (this->configFile);
}

int	Config::getNbServers(void) {
	return (this->nbServer);
}
