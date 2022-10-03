/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:50:45 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/03 14:05:25 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	countLenServer(std::istream &file) {
	std::string	line;
	int			count = 0;

	while (std::getline(file, line)) {
		if (line == "server {")
			count++;
	}
	return (count);
}

int	ft_parse_server(std::istream &file, std::map<int, std::map<std::string, std::string>> &configFile) {
	std::string line;
	int			notClosed;
	int			serverNumber;
	
	notClosed = 0;
	serverNumber = -1;
	while (std::getline(file, line)) {
		if (line == "server {") {
			serverNumber++;
			std::map<std::string, std::string>	serverParams;
			while (std::getline(file, line)) {
				if (line == "}")
				{
					notClosed = 1;
					break;
				}
				std::vector<std::string> parsedLine = split(line, "\t");
				print_tab(parsedLine);
				if (parsedLine.size() != 2)
				{
					perror("error: format not respected in config file");
					return (FALSE);
				}
				serverParams.insert(std::make_pair(parsedLine[0], parsedLine[1]));
			}
			if (notClosed == 0)
			{
				perror("error: '{' not closed in config file");
				return (FALSE);
			}
			configFile.insert(std::make_pair(serverNumber, serverParams));
		}
	}
	return (TRUE);
}
