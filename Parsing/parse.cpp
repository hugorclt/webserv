/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:20:23 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/22 18:48:27 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	parseStartLine(char **tab, std::map<std::string, std::vector<std::string>> &map) {
	char	**start_line = ft_split(tab[0], " ");
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
	ft_free(start_line);
	return (TRUE);
}

int	parseHeaders(char **tab, std::map<std::string, std::vector<std::string>> &map)
{
	char	*key;
	char	**value;
			
	for (int i = 1; i < getTabLen(tab) - 1; i++) {
		key = ft_strndup(tab[i], getKeyLen(tab[i]));
		value = ft_split_value(tab[i]);
		std::vector<std::string> vec;
		for (int j = 0; value[j]; j++)
			vec.push_back(std::string(value[j]));
		map.insert(std::make_pair(key, vec));
		ft_free(value);
		free(key);
	}
	return (TRUE);
}

std::map<std::string, std::vector<std::string>>	createHttpRequest(const char	*buffer) {
	std::map<std::string, std::vector<std::string>>	res;
	char	**tab = ft_split(buffer, "\n");
	
	// Start line
	parseStartLine(tab, res);

	// Header
	parseHeaders(tab, res);
	
//	std::cout << res["Accept"][0] << std::endl;
	ft_free(tab);
	return (res);
}
