/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/03 14:02:54 by hrecolet         ###   ########.fr       */
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
