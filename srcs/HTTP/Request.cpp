/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 11:32:35 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/20 13:28:20 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(std::string &req)
{
	std::vector<std::string> request = split(req, "\r\n");

	_parseFirstLine(request);
	std::vector<std::string>::iterator itReq = request.begin() + 1;
	for (; itReq != request.end() && *itReq != "\r\n"; itReq++)
	{
		_basicSplit(*itReq);
	}

	for (; itReq != request.end(); itReq++)
	{
		for (std::string::iterator itLine = itReq->begin(); itLine != itReq->end(); itLine++)
			_body.push_back(*itLine);
	}
	//_printValue();
}

Request::~Request(void) {

}

void Request::_parseFirstLine(std::vector<std::string> &request)
{
	std::vector<std::string> line = split(request[0], " ");
	_method = line[0];
	_version = line[2];
	size_t pos = line[1].find_first_of("?");
	if (pos == std::string::npos)
		pos = line[1].length();
	_target = line[1].substr(0, pos);
	if (pos == line[1].length())
		pos--;
	std::string var = line[1].substr(pos + 1, line[1].length());
	_var = split(var, "&");
}

void	Request::_printValue(void)
{
	std::cout << "----------firstLine----------" << std::endl;
	std::cout << _method << std::endl
		<< _target << std::endl
		<< _version << std::endl;
	std::cout << "----------var-----------" << std::endl;
	for (std::vector<std::string>::iterator it = _var.begin(); it != _var.end(); it++)
	{
		std::cout << *it << std::endl;
	}
	std::cout << "----------Header-----------" << std::endl;
	for (request_type::iterator it = _header.begin(); it != _header.end(); it++)
	{
		std::cout << it->first << ":";
		for (std::vector<std::string>::iterator itline = it->second.begin(); itline != it->second.end(); itline++)
			std::cout << *itline << " ";
		std::cout << std::endl;
	}
	std::cout << "----------body-----------" << std::endl;
	for (std::vector<char>::iterator it = _body.begin(); it != _body.end(); it++)
	{
		std::cout << *it;
	}
}

void	Request::_basicSplit(std::string &line)
{
	std::vector<std::string> lineSplited = split(line, ":");

	std::string key = lineSplited[0];
	std::vector<std::string> value = split_charset(lineSplited[1], ", ");
	if (key == "Host")
	{
		value.push_back(lineSplited[2]);
	}
	_header.insert(std::make_pair(key, value));
}

// void	Request::_acceptSplit(std::string &line)
// {
// 	std::vector<std::string> lineSplited = split(line, ":");

// 	std::string key = lineSplited[0];
// 	std::vector<std::string> value = split(lineSplited[1], ",");
// 	_header.insert({key, value});
// }

// void	Request::_Split(std::string &line)
// {
// 	std::vector<std::string> lineSplited = split(line, ":");

// 	std::string key = lineSplited[0];
// 	std::vector<std::string> value = split(lineSplited[1], ",");
// 	_header.insert({key, value});
// }

std::vector<std::string>	&Request::getVar(void) {
	return (_var);
}

Request::request_type	Request::getData(void) const {
	return (_header);
}

std::string	Request::getTarget(void) const {
	return (_target);
}

std::string	Request::getMethod(void) const {
	return (_method);
}

std::string	Request::getVersion(void) const {
	return (_version);
}

std::vector<char>	Request::getBody(void) const {
	return (_body);
}