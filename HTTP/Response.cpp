/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:23:33 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 16:25:07 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/* -------------------------------------------------------------------------- */
/*                                 constructor                                */
/* -------------------------------------------------------------------------- */

Response::Response(HTTPRequest &req, Server *server) : _req(req) , _server(server)
{	
}

/* -------------------------------------------------------------------------- */
/*                                   methods                                  */
/* -------------------------------------------------------------------------- */

std::string	_getDefaultErrorPage(void)
{
	return ("<center><h2>backup error pages" + _code + "</h2></center>");
}

std::string	Response::_getDate(void) {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];
	
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (buffer,80,"%a, %e %b %G %T GMT",timeinfo);
	return (std::string(buffer));
}

void	Response::_constructBody(void)
{
	std::string	target = root + _req.getData()["target"][0];

	std::ifstream in(target, ::std::ios::binary);
	_code = 200;
	if (!in.good())
	{
		std::cout << errno << std::endl;
		std::ifstream in(_server->getError() + "404.html", ::std::ios::binary);
		_code = 404;
		if (!in.good())
		{
			std::string errorPage = _getDefaultErrorPage();
			_data.insert(errorPage.begin(), errorPage.end());
			return ;
		}
	}
	while (in) {
		char c;
		in.get(c);
		_data.push_back(c);
	}
}

void	Response::_constructHeader(void)
{
	
}

void	Response::construct(void) {
	_constructBody();
	_constructHeader();
}

void	Request::send(void)
{
	if (send(clientFd, _data.data(), _data.size(), 0) == -1)
        perror("send error:");
}











/*
HTTPResponse::HTTPResponse(HTTPRequest &req, ServerList &servers) {
	std::map<std::string, std::vector<std::string>> dataReq = req.getData();
	std::vector<std::string> hostIp (dataReq["Host"].rbegin(), dataReq["Host"].rend());
    ServerList::serverValue::iterator serverIt = servers.getServerByIpPort(hostIp);
    
    int code = execRequest(req , *this, (*serverIt)->getRoot());
	this->data.insert(std::make_pair("version", dataReq["httpVersion"][0]));
	this->data.insert(std::make_pair("date", getDate()));
	this->data.insert(std::make_pair("server", "webserv/0.1"));
    this->data.insert(std::make_pair("code", to_string(code)));
    this->data.insert(std::make_pair("status", "OK"));
}

void     HTTPResponse::sendRequest(int clientFd) {
    std::string         header;

    header = this->data["version"] + " " + this->data["code"] + " " + this->data["status"] + "\r\n"
        + "Date: " + this->data["date"] + "\r\n"
        + "Server: " + this->data["server"] + "\r\n"
        + "Content-Length: " + to_string(this->body.size()) + "\r\n"
        // + "Content-Type: " + this->data["type"] + "\r\n"
        + "Content-Type: text/html\r\n" 
        + "Connection: Closed" + "\r\n"
        + "\n";

    this->body.insert(this->body.begin(), header.begin(), header.end());
    if (send(clientFd, this->body.data(), this->body.size(), 0) == -1)
        perror("send error:");
}

void    HTTPResponse::setBody(std::vector<char> body) {
    this->body = body;
}

void    HTTPResponse::setLength(int length) {
    this->length = length;
}
*/
