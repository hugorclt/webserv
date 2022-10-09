/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 16:46:24 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/09 17:01:20 by hrecolet         ###   ########.fr       */
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
	return (std::string(buffer));
}

void	execGet(HTTPRequest	&req, HTTPResponse &res, std::string root) {
	std::string	target = root + req.getData()["target"][0];
	std::ifstream in(target, ::std::ios::binary);
	std::vector<char> html;
	
	if (!in.good()) {
		//std::ifstream in(, ::std::ios::binary);
	}
	while (in) {
		char c;
		in.get(c);
		html.push_back(c);
	}
	res.setBody(html);
}

int	execRequest(HTTPRequest	&req, HTTPResponse &res, std::string root) {
	std::map<std::string, std::vector<std::string>>	reqData = req.getData();
	
	if (reqData["methods"][0] == "GET")
		execGet(req, res, root);
	return (0);
}
