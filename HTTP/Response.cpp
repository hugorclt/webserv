/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:23:33 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/18 17:20:10 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

const std::map<std::string, std::string>	Response::_mimeTypes {
	{".aac", "audio/aac"},
	{".abw", "application/x-abiword"},
	{".arc", "application/octet-stream"},
	{".avo", "video/x-msvideo"},
	{".bin", "application/octet-stream"},
	{".bmp", "image/bmp"},
	{".bz", "application/x-bzip"},
	{".bz2", "application/x-bzip2"},
	{".csh", "application/x-csh"},
	{".css", "text/css"},
	{".csv", "text/csv"},
	{".doc", "application/msword"},
	{".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
	{".eot", "application/vnd.ms-fontobject"},
	{".epub", "application/epub+zip"},
	{".gif", "image/gif"},
	{".htm", "text/html"},
	{".html", "text/html"},
	{".ico", "image/x-icon"},
	{".ics", "text/calendar"},
	{".jar", "application/java-archive"},
	{".jpeg", "image/jpeg"},
	{".jpg", "image/jpeg"},
	{".js", "application/javascript"},
	{".json", "application/json"},
	{".mid", "audio/midi"},
	{".midi", "audio/midi"},
	{".mpeg", "video/mpeg"},
	{".mpkg", "application/vnd.apple.installer+xml"},
	{".odp", "application/vnd.oasis.opendocument.presentationa"},
	{".ods", "application/vnd.oasis.opendocument.spreadsheet"},
	{".odt", "application/vnd.oasis.opendocument.text"},
	{".oga", "audio/ogg"},
	{".ogv", "video/ogg"},
	{".ogx", "application/ogg"},
	{".otf", "font/otf"},
	{".png", "image/png"},
	{".pdf", "application/pdf"},
	{".ppt", "application/vnd.ms-powerpoint"},
	{".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
	{".rar", "application/x-rar-compressed"},
	{".rtf", "application/rtf"},
	{".sh", "application/x-sh"},
	{".svg", "image/svg+xml"},
	{".swf", "application/x-shockwave-flash"},
	{".tar", "application/x-tar"},
	{".tif", "image/tiff"},
	{".tiff", "image/tiff"},
	{".ts", "application/typescript"},
	{".ttf", "font/ttf"},
	{".vsd", "application/vnd.visio"},
	{".wav", "audio/x-wav"},
	{".weba", "audio/webm"},
	{".webm", "video/webm"},
	{".webp", "image/webp"},
	{".woff", "font/woff"},
	{".woff2", "font/woff2"},
	{".xhtml", "application/xhtml+xml"},
	{".xls", "application/vnd.ms-excel"},
	{".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
	{".xml", "application/xml"},
	{".xul", "application/vnd.mozilla.xul+xml"},
	{".zip", "application/zip"},
	{".3gp", "video/3gpp"},
	{".3g2", "video/3gpp2"},
	{".7z", "application/x-7z-compressed"}
};

const std::map<std::string, void(Response::*)()>	Response::_methodsFunction
{
	{"GET", &Response::_execGet},
	{"POST", NULL},
	{"DELETE", NULL}
};

/* -------------------------------------------------------------------------- */
/*                                 constructor                                */
/* -------------------------------------------------------------------------- */

Response::Response(ConfigParser::Location &env, HTTPRequest &req) : _env(env), _req(req)
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

// void	Response::_constructBody(void)
// {
// 	std::string	target = root + _req.getData()["target"][0];

// 	std::ifstream in(target, ::std::ios::binary);
// 	_code = 200;
// 	if (!in.good())
// 	{
// 		std::cout << errno << std::endl;
// 		std::ifstream in(_server->getError() + "404.html", ::std::ios::binary);
// 		_code = 404;
// 		if (!in.good())
// 		{
// 			std::string errorPage = _getDefaultErrorPage();
// 			_data.insert(errorPage.begin(), errorPage.end());
// 			return ;
// 		}
// 	}

// }

bool	Response::is_file_accessible(std::string filename)
{
	struct stat buffer;   
	return (stat(filename.c_str(), &buffer) == 0); 
}

bool	Response::is_file_opened(std::ifstream file)
{
	return (file.is_open());
}

/*
root ./bite
location /coucou/hugo
	root ./lol

target = /coucou/hugo/caca/index.html
./lol/caca/index.html
*/
void	Response::_execGet(void) {
	std::string root = _req.getData()["target"][0].erase(0, (*(_env.uniqKey["_rootToDel_"].begin())).length());
	root = *(_env.uniqKey["root"].begin()) + root;
}

void	Response::_readFile(std::ifstream file)
{
	while (file) 
	{
		char c;
		file.get(c);
		_data.push_back(c);
	}
}


void	Response::execute(void) {
	std::string method = _req.getData()["methods"][0];
	if (_env.uniqKey["allow_methods"].count(method))
	{
		(this->*(_methodsFunction.find(method)->second))();
		return ;	
	}
	_code = "405";
	_status = (*_env.nonUniqKey["return"][_code].begin());
	return ;
}

// void	Response::send(int clientFd)
// {
// 	if (send(clientFd, _data.data(), _data.size(), 0) == -1)
//         perror("send error:");
// }

bool	Response::_isBinaryFile(std::string filePath)
{
	int	character;
	
	std::ifstream file(filePath);
	while ((character = file.get()) != EOF && character <= 127)
		;
	if (character == EOF)
		return (false);
	return (true);
}

void	Response::_setType(std::string url)
{
	std::string	extension = url.substr(url.find_last_of(".") + 1);
	std::map<std::string, std::string>::const_iterator it = _mimeTypes.find(extension);

	if (it != _mimeTypes.end())
	{
		_types = it->second;
		return ;
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
