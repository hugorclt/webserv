/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 16:46:24 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/08 18:09:57 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string	getDate(void) {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];
	
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (buffer,80,"%a, %e %b %G %T GMT",timeinfo);
	std::cout << buffer << std::endl;
	return 0;
}

void	execGet(HTTPRequest	&req, HTTPResponse &res) {
	
}

int	execRequest(HTTPRequest	&req, HTTPResponse &res, std::string root) {
	std::map<std::string, std::vector<std::string>>	reqData = req.getData();
	
	if (reqData["methods"][0] == "GET")
		execGet(req, res);
}
