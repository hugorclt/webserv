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
#include "utils.hpp"
#include <iostream>
#include <algorithm>


void	Request::_parseHeader(std::string &header)
{
	std::vector<std::string>	vecHeader = split(header, "\r\n");
	std::vector<std::string>::iterator itReq = vecHeader.begin();
	for (; itReq != vecHeader.end(); itReq++)
	{
		_basicSplit(*itReq);
	}
}

void	Request::_unchunkedRequest(std::vector<char> &body)
{
	std::vector<char>::iterator it = body.begin();

	while (it != body.end())
	{
		it = body.erase(it, _vectorCharSearch(it, body.end(), "\r\n"));
		it = _vectorCharSearch(it, body.end(), "\r\n");
	}
}

void	Request::_parseFileName(std::vector<char> &body)
{
	std::vector<char>::iterator	fileNameIt = _vectorCharSearch(body.begin(), body.end(), "filename=\"");
	std::string _uploadFileName (fileNameIt, _vectorCharSearch(fileNameIt, body.end(), "\""));

	for (int i = 0; i < 3; i++)
		body.erase(body.begin(), _vectorCharSearch(body.begin(), body.end(), "\r\n"));
}

void	Request::_parseBody(std::vector<char> &body)
{
	if (_method != "POST")
		return ;
	if (_header.count("Transfer-Encoding") && !_header["Transfer-Encoding"].empty() && _header["Transfer-Encoding"][0] == "chunked")
		_unchunkedRequest(body);
	if (_header["Content-Type"][0] == "application/x-www-form-urlencoded")
		_envVar.insert(_envVar.end(), body.begin(), body.end());
	else
	{
		_parseFileName(body);
		_body = body;
	}
}

std::vector<char>::iterator	Request::_vectorCharSearch(std::vector<char>::iterator first, std::vector<char>::iterator last, std::string toFind)
{
	return (std::search(first, last, toFind.begin(), toFind.end()) + toFind.size());
}

Request::Request(std::vector<char> &req)
{
	std::string 		firstLine(req.begin(), _vectorCharSearch(req.begin(), req.end(), "\r\n"));
	std::string		 	header(_vectorCharSearch(req.begin(), req.end(), "\r\n"), _vectorCharSearch(req.begin(), req.end(), "\r\n\r\n"));
	std::vector<char>	body( _vectorCharSearch(req.begin(), req.end(), "\r\n\r\n"), req.end());

	std::cout << firstLine << std::endl;
	std::cout << header << std::endl;

	_parseFirstLine(firstLine);
	_parseHeader(header);
	if (_header.count("Content-Length") && !_header["Content-Length"].empty() && atoi(_header["Content-Length"][0].c_str()) != static_cast<int>(body.size()))
		throw std::bad_alloc();
	_parseBody(body);
	_printValue();
	std::cout << _uploadFileName << std::endl;
}

Request::~Request(void) {

}

void Request::_parseFirstLine(std::string &request)
{
	std::vector<std::string> line = split_charset(request, " \r\n");
	_method = line[0];
	_version = line[2];
	size_t pos = line[1].find_first_of("?");
	if (pos == std::string::npos)
		pos = line[1].length();
	_target = line[1].substr(0, pos);
	if (pos == line[1].length())
		pos--;
	std::string var = line[1].substr(pos + 1, line[1].length());
	_argvVar = split(var, "&");
}

void	Request::_printValue(void)
{
	std::cout << "----------firstLine----------" << std::endl;
	std::cout << _method << std::endl
		<< _target << std::endl
		<< _version << std::endl;
	std::cout << "----------var-----------" << std::endl;
	for (std::vector<std::string>::iterator it = _argvVar.begin(); it != _argvVar.end(); it++)
	{
		std::cout << *it << std::endl;
	}
	std::cout << "----------envVar-----------" << std::endl;
	std::cout << _envVar << std::endl;
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
		//std::cout << *it;
	}
	std::cout << std::endl;
}

void	Request::_basicSplit(std::string &line)
{
	std::vector<std::string> lineSplited = split(line, ":");

	std::string key = lineSplited[0];
	std::vector<std::string> value = split_charset(lineSplited[1], ", ;");
	if (key == "Host")
	{
		value.push_back(lineSplited[2]);
	}
	_header.insert(std::make_pair(key, value));
}

std::vector<std::string>	&Request::getVar(void) {
	return (_argvVar);
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

std::string		Request::getEnvVar(void) {
	return (_envVar);
}

std::string		Request::getUploadFileName(void) {
	return (_uploadFileName);
}
