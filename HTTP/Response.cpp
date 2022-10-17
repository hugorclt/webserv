/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:23:33 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/18 00:30:04 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

const std::pair<std::string, std::string>	Response::_mimeTypes[NB_MIME] = {
	std::make_pair((".aac"), ("audio/aac")),
	std::make_pair((".abw"), ("application/x-abiword")),
	std::make_pair((".arc"), ("application/octet-stream")),
	std::make_pair((".avo"), ("video/x-msvideo")),
	std::make_pair((".bin"), ("application/octet-stream")),
	std::make_pair((".bmp"), ("image/bmp")),
	std::make_pair((".bz"), ("application/x-bzip")),
	std::make_pair((".bz2"), ("application/x-bzip2")),
	std::make_pair((".csh"), ("application/x-csh")),
	std::make_pair((".css"), ("text/css")),
	std::make_pair((".csv"), ("text/csv")),
	std::make_pair((".doc"), ("application/msword")),
	std::make_pair((".docx"), ("application/vnd.openxmlformats-officedocument.wordprocessingml.document")),
	std::make_pair((".eot"), ("application/vnd.ms-fontobject")),
	std::make_pair((".epub"), ("application/epub+zip")),
	std::make_pair((".gif"), ("image/gif")),
	std::make_pair((".htm"), ("text/html")),
	std::make_pair((".html"), ("text/html")),
	std::make_pair((".ico"), ("image/x-icon")),
	std::make_pair((".ics"), ("text/calendar")),
	std::make_pair((".jar"), ("application/java-archive")),
	std::make_pair((".jpeg"), ("image/jpeg")),
	std::make_pair((".jpg"), ("image/jpeg")),
	std::make_pair((".js"), ("application/javascript")),
	std::make_pair((".json"), ("application/json")),
	std::make_pair((".mid"), ("audio/midi")),
	std::make_pair((".midi"), ("audio/midi")),
	std::make_pair((".mpeg"), ("video/mpeg")),
	std::make_pair((".mpkg"), ("application/vnd.apple.installer+xml")),
	std::make_pair((".odp"), ("application/vnd.oasis.opendocument.presentationa")),
	std::make_pair((".ods"), ("application/vnd.oasis.opendocument.spreadsheet")),
	std::make_pair((".odt"), ("application/vnd.oasis.opendocument.text")),
	std::make_pair((".oga"), ("audio/ogg")),
	std::make_pair((".ogv"), ("video/ogg")),
	std::make_pair((".ogx"), ("application/ogg")),
	std::make_pair((".otf"), ("font/otf")),
	std::make_pair((".png"), ("image/png")),
	std::make_pair((".pdf"), ("application/pdf")),
	std::make_pair((".ppt"), ("application/vnd.ms-powerpoint")),
	std::make_pair((".pptx"), ("application/vnd.openxmlformats-officedocument.presentationml.presentation")),
	std::make_pair((".rar"), ("application/x-rar-compressed")),
	std::make_pair((".rtf"), ("application/rtf")),
	std::make_pair((".sh"), ("application/x-sh")),
	std::make_pair((".svg"), ("image/svg+xml")),
	std::make_pair((".swf"), ("application/x-shockwave-flash")),
	std::make_pair((".tar"), ("application/x-tar")),
	std::make_pair((".tif"), ("image/tiff")),
	std::make_pair((".tiff"), ("image/tiff")),
	std::make_pair((".ts"), ("application/typescript")),
	std::make_pair((".ttf"), ("font/ttf")),
	std::make_pair((".vsd"), ("application/vnd.visio")),
	std::make_pair((".wav"), ("audio/x-wav")),
	std::make_pair((".weba"), ("audio/webm")),
	std::make_pair((".webm"), ("video/webm")),
	std::make_pair((".webp"), ("image/webp")),
	std::make_pair((".woff"), ("font/woff")),
	std::make_pair((".woff2"), ("font/woff2")),
	std::make_pair((".xhtml"), ("application/xhtml+xml")),
	std::make_pair((".xls"), ("application/vnd.ms-excel")),
	std::make_pair((".xlsx"), ("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")),
	std::make_pair((".xml"), ("application/xml")),
	std::make_pair((".xul"), ("application/vnd.mozilla.xul+xml")),
	std::make_pair((".zip"), ("application/zip")),
	std::make_pair((".3gp"), ("video/3gpp")),
	std::make_pair((".3g2"), ("video/3gpp2")),
	std::make_pair((".7z"), ("application/x-7z-compressed")),

};

/* -------------------------------------------------------------------------- */
/*                                 constructor                                */
/* -------------------------------------------------------------------------- */

Response::Response(HTTPRequest &req, Server *server) : _req(req) , _server(server)
{	
}

/* -------------------------------------------------------------------------- */
/*                                   methods                                  */
/* -------------------------------------------------------------------------- */

std::string	Response::_getDefaultErrorPage(void)
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

bool	Response::_isBinaryFile(std::string filePath)
{
	int	character;
	
	std::ifstream file(filePath);
	while ((character = file.get()) != EOF && c <= 127)
		;
	if (c == EOF)
		return (false);
	return (true);
}

void	Response::_setType(std::string url)
{
	std::string	extension = url.substr(url.find_last_of(".") + 1);
	
	for (int i = 0; i < NB_MIME; i++)
	{
		if (extension == _mimeTypes[i].first)
		{
			_types = _mimeTypes[i].second;
			return ;
		}
	}
	if (_isBinaryFile(url))
		_types = "application/octet-stream";
	else
		_types = "text/plain";
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
