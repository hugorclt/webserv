/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                       :+:      :+:    :+:    */
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
#include <sys/socket.h>
#include <cstring>

size_t	Request::_maxHeaderSize = 500;

bool	Request::_isHeaderComplete(void)
{ return (_vectorCharSearch(_rawData.begin(), _rawData.end(), "\r\n\r\n") != _rawData.end()); }

void	Request::_fillHeader(void)
{
	std::string	firstLine(_rawData.begin(), _vectorCharSearch(_rawData.begin(), _rawData.end(), "\r\n"));
	_parseFirstLine(firstLine);
	_rawData.erase(_rawData.begin(), _rawData.begin() + firstLine.size() + 2);

	std::string	header(_rawData.begin(), _vectorCharSearch(_rawData.begin(), _rawData.end(), "\r\n\r\n"));
	_parseHeader(header);
	_checkHeader();
	_rawData.erase(_rawData.begin(), _rawData.begin() + header.size() + 4);
}

bool	Request::_isBodyComplete(void)
{
	if (!_header.count("Content-Length") && !_rawData.empty())
		throw RequestError("No Content-Length but start of a body");
	if (!_header.count("Content-Length") && _rawData.empty())
		return (true);

	int contentLength = atoi(_header["Content-Length"][0].c_str());

	if (contentLength < 0)
		throw RequestError("Negative Content-Length");
	if (_rawData.size() > static_cast<size_t>(contentLength))
		throw RequestError("Body larger than Content-Length");
	return (static_cast<size_t>(contentLength) == _rawData.size());
}

void	Request::_fillBody(void)
{ _parseBody(_rawData); }

bool	Request::rec(void)
{
	char	buffer[4096];
	int		readedBytes;

	memset(buffer, 0, 4096);
	readedBytes = recv(_fd, buffer, 4096, 0);
	std::cout << "-------------------" << std::endl;
	std::cout << buffer << std::endl;
	std::cout << "-------------------" << std::endl;
	if (readedBytes == -1)
		throw RequestError("recv return == -1");
	_rawData.insert(_rawData.end(), buffer, buffer + readedBytes);
	if (_header.empty() && !_isHeaderComplete() && _rawData.size() > _maxHeaderSize)
		throw InvalidHeader("Invalid header (larger than _maxHeaderSize)");
	//std::cout << "_isHeaderComplete : " << _isHeaderComplete() << std::endl;
	if (_header.empty() && _isHeaderComplete())
		_fillHeader();
	if (!_header.empty() && _isBodyComplete())
		_fillBody();
	//std::cout << "-------------------" << std::endl;
	//std::cout << std::string(buffer, buffer + readedBytes) << std::endl;
	//std::cout << "-------------------" << std::endl;
	return (!_header.empty() && _isBodyComplete());
}

void	Request::_parseHeader(std::string &header)
{
	std::vector<std::string>	vecHeader = split(header, "\r\n");
	for (std::vector<std::string>::iterator itReq = vecHeader.begin(); itReq != vecHeader.end(); itReq++)
		_basicSplit(*itReq);
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
	std::string test (fileNameIt, _vectorCharSearch(fileNameIt, body.end(), "\"") - 1);

	_uploadFileName = test;
	for (int i = 0; i < 4; i++)
		body.erase(body.begin(), _vectorCharSearch(body.begin(), body.end(), "\r\n"));
	body.erase(_searchLastLine(body), body.end());
}

std::vector<char>::iterator	Request::_searchLastLine(std::vector<char> &body)
{
	std::vector<char>::iterator ite = body.end();

	ite -= 2;
	while (*ite != '\n')
	{
		ite--;
	}
	ite--;
	return (ite);
}

void	Request::_parseBody(std::vector<char> &body)
{
	if (_method != "POST")
		return ;
	if (_header.count("Transfer-Encoding") && !_header["Transfer-Encoding"].empty() && _header["Transfer-Encoding"][0] == "chunked")
		_unchunkedRequest(body);
	if (_header.count("Content-Type") && !_header["Content-Type"].empty() && !_header["Content-Type"][0].empty() && _header["Content-Type"][0] == "application/x-www-form-urlencoded")
		_envVar.insert(_envVar.end(), body.begin(), body.end());
	else if (_header.count("Content-Type") && !_header["Content-Type"].empty() && !_header["Content-Type"][0].empty())
	{
		std::vector<std::string>	content_type = split_charset(_header["Content-Type"][0], "; ");
		if (content_type[0] == "multipart/form-data" && _header.count("Content-Length") && !_header["Content-Length"].empty() && _header["Content-Length"][0] != "0")
		{
			_parseFileName(body);
		}
		_body = body;
	}
	else
		_body = body;
}

std::vector<char>::iterator	Request::_vectorCharSearch(std::vector<char>::iterator first, std::vector<char>::iterator last, std::string toFind)
{ return (std::search(first, last, toFind.begin(), toFind.end())); }

Request::Request(int fd)
: _fd(fd) {}


/* old constructor
Request::Request(std::vector<char> &req)
{
	std::string 		firstLine(req.begin(), _vectorCharSearch(req.begin(), req.end(), "\r\n"));
	std::string		 	header(_vectorCharSearch(req.begin(), req.end(), "\r\n"), _vectorCharSearch(req.begin(), req.end(), "\r\n\r\n"));
	std::vector<char>	body(_vectorCharSearch(req.begin(), req.end(), "\r\n\r\n"), req.end());

	_parseFirstLine(firstLine);
	_parseHeader(header);
	_checkHeader();
	//if (_header.count("Content-Length") && !_header["Content-Length"].empty() && static_cast<size_t>(atoi(_header["Content-Length"][0].c_str())) != body.size())
	//	throw RequestError("Content-Length not the same as the body size");
	_parseBody(body);
	// _printValue();
}
*/

void	Request::_checkHeader(void)
{
	if (!_header.count("Host") && _header["Host"].empty() && _header["host"].size() != 2)
		throw InvalidHeader("Invalid header");
	if (_method == "POST" && (!_header.count("Content-Type") || _header["Content-Type"].empty()))
		throw InvalidHeader("Invalid header");

}

Request::~Request(void) {

}

void	Request::_parseFirstLine(const std::string &line)
{
	std::vector<std::string> vec = split_charset(line, " ");
	if (vec.size() != 3)
		throw RequestError("first header Line");
	_method = vec[0];
	_version = vec[2];
	size_t pos = vec[1].find_first_of("?");
	if (pos == std::string::npos)
		pos = vec[1].size();
	_target = vec[1].substr(0, pos);
	if (pos == vec[1].size())
		pos--;
	std::string var = vec[1].substr(pos + 1, vec[1].length());
	_argvVar = split(var, "&");
}

void	Request::_printValue(void)
{
	std::cout << "----------nameFile----------" << std::endl;
	std::cout << _uploadFileName << std::endl;
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
	if (lineSplited.size() <= 2 && lineSplited[0] == "Host")
		throw InvalidHeader("Host invalid");
	std::string key = lineSplited[0];
	std::vector<std::string> value;

	if (key == "Content-Type")
		value = split_charset(lineSplited[1], ", ;");
	else if (key == "Host")
	{
		value.push_back(lineSplited[1]);
		value.push_back(lineSplited[2]);
		value[0].erase(value[0].begin());
	}
	else
	{
		value.push_back(lineSplited[1]);
		value[0].erase(value[0].begin());
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
