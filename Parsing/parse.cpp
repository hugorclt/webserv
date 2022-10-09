/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:20:23 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/09 12:49:47 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	parseStartLine(std::string	firstLine, std::map<std::string, std::vector<std::string>> &map) {
	std::vector<std::string>	start_line = split(firstLine, " ");
	for (int i = 0; i < 3; i++) {
		std::vector<std::string> vec;
		vec.push_back(std::string(start_line[i]));
		if (i == 0)
			map.insert(std::make_pair("methods", vec));
		else if (i == 1)
			map.insert(std::make_pair("target", vec));
		else
			map.insert(std::make_pair("httpVersion", vec));
	}
	return (TRUE);
}

void	parseHost(std::string tab, std::map<std::string, std::vector<std::string>> &map) {
	std::vector<std::string>	host = split(tab, ":");
	
	std::string	key = host[0];
	std::string	hostIp = host[1].substr(1, host[1].length());
	std::string	port = host[2];

	std::vector<std::string> vec;
	vec.push_back(hostIp);
	vec.push_back(port);

	map.insert(std::make_pair(key, vec));
}

int	parseHeaders(std::vector<std::string> tab, std::map<std::string, std::vector<std::string>> &map) {
	std::string					key;
			
	for (int i = 2; i < tab.size(); i++) {
		std::vector<std::string>	value;
		key = tab[i].substr(0, tab[i].find(":"));
		if (key == "Accept" || key == "Accept-Language")
			value = split(tab[i], ',', tab[i].find(":") + 2);
		else
			value = split(tab[i], ' ', tab[i].find(":") + 2);
		std::vector<std::string> vec;
		for (int j = 0; j < value.size(); j++)
			vec.push_back(value[j]);
		map.insert(std::make_pair(key, vec));
	}
	return (0);
}

std::map<std::string, std::vector<std::string>>	createHttpRequest(std::string req) {
	std::map<std::string, std::vector<std::string>>	res;
	std::vector<std::string>	tab = split(req, "\r\n");
	
	parseStartLine(tab[0], res);
	parseHost(tab[1], res);
	parseHeaders(tab, res);
	
//	std::cout << res["Accept"][0] << std::endl;
	return (res);
}
