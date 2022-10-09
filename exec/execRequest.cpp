/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 16:46:24 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/09 13:37:10 by hrecolet         ###   ########.fr       */
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
	
	std::ifstream t(target);
	std::stringstream html_code;
	html_code << t.rdbuf();
	std::string	html(html_code.str());
	
	//virer le PUTAIN DE /n
	html.erase(std::remove(html.begin(), html.end(), '\n'),
            html.end());
	
	res.setBody(html);
	res.setLength(html.length());
	// res.setType()
}

int	execRequest(HTTPRequest	&req, HTTPResponse &res, std::string root) {
	std::map<std::string, std::vector<std::string>>	reqData = req.getData();
	
	if (reqData["methods"][0] == "GET")
		execGet(req, res, root);
	return (0);
}
