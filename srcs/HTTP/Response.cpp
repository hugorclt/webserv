/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:23:33 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/19 14:30:42 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)

void	Response::init_mimeTypes(void)
{
	if (!_mimeTypes.empty())
		return ;
	std::pair<std::string, std::string>	mimeTypes[] =
	{
		std::make_pair(".aac", "audio/aac"),
		std::make_pair(".abw", "application/x-abiword"),
		std::make_pair(".arc", "application/octet-stream"),
		std::make_pair(".avo", "video/x-msvideo"),
		std::make_pair(".bin", "application/octet-stream"),
		std::make_pair(".bmp", "image/bmp"),
		std::make_pair(".bz", "application/x-bzip"),
		std::make_pair(".bz2", "application/x-bzip2"),
		std::make_pair(".csh", "application/x-csh"),
		std::make_pair(".css", "text/css"),
		std::make_pair(".csv", "text/csv"),
		std::make_pair(".doc", "application/msword"),
		std::make_pair(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"),
		std::make_pair(".eot", "application/vnd.ms-fontobject"),
		std::make_pair(".epub", "application/epub+zip"),
		std::make_pair(".gif", "image/gif"),
		std::make_pair(".htm", "text/html"),
		std::make_pair(".html", "text/html"),
		std::make_pair(".ico", "image/x-icon"),
		std::make_pair(".ics", "text/calendar"),
		std::make_pair(".jar", "application/java-archive"),
		std::make_pair(".jpeg", "image/jpeg"),
		std::make_pair(".jpg", "image/jpeg"),
		std::make_pair(".js", "application/javascript"),
		std::make_pair(".json", "application/json"),
		std::make_pair(".mid", "audio/midi"),
		std::make_pair(".midi", "audio/midi"),
		std::make_pair(".mpeg", "video/mpeg"),
		std::make_pair(".mpkg", "application/vnd.apple.installer+xml"),
		std::make_pair(".odp", "application/vnd.oasis.opendocument.presentationa"),
		std::make_pair(".ods", "application/vnd.oasis.opendocument.spreadsheet"),
		std::make_pair(".odt", "application/vnd.oasis.opendocument.text"),
		std::make_pair(".oga", "audio/ogg"),
		std::make_pair(".ogv", "video/ogg"),
		std::make_pair(".ogx", "application/ogg"),
		std::make_pair(".otf", "font/otf"),
		std::make_pair(".png", "image/png"),
		std::make_pair(".pdf", "application/pdf"),
		std::make_pair(".ppt", "application/vnd.ms-powerpoint"),
		std::make_pair(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"),
		std::make_pair(".rar", "application/x-rar-compressed"),
		std::make_pair(".rtf", "application/rtf"),
		std::make_pair(".sh", "application/x-sh"),
		std::make_pair(".svg", "image/svg+xml"),
		std::make_pair(".swf", "application/x-shockwave-flash"),
		std::make_pair(".tar", "application/x-tar"),
		std::make_pair(".tif", "image/tiff"),
		std::make_pair(".tiff", "image/tiff"),
		std::make_pair(".ts", "application/typescript"),
		std::make_pair(".ttf", "font/ttf"),
		std::make_pair(".vsd", "application/vnd.visio"),
		std::make_pair(".wav", "audio/x-wav"),
		std::make_pair(".weba", "audio/webm"),
		std::make_pair(".webm", "video/webm"),
		std::make_pair(".webp", "image/webp"),
		std::make_pair(".woff", "font/woff"),
		std::make_pair(".woff2", "font/woff2"),
		std::make_pair(".xhtml", "application/xhtml+xml"),
		std::make_pair(".xls", "application/vnd.ms-excel"),
		std::make_pair(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"),
		std::make_pair(".xml", "application/xml"),
		std::make_pair(".xul", "application/vnd.mozilla.xul+xml"),
		std::make_pair(".zip", "application/zip"),
		std::make_pair(".3gp", "video/3gpp"),
		std::make_pair(".3g2", "video/3gpp2"),
		std::make_pair(".7z", "application/x-7z-compressed")
	};
	_mimeTypes.insert(mimeTypes, mimeTypes + SIZEOF(mimeTypes));
}

void	Response::init_methodsFunction(void)
{
	if (!_methodsFunction.empty())
		return ;
	//FIX THIS SHIT LATER (function pointer can't be NULL right now)
	std::pair<std::string, void(Response::*)()>	methodsFunction[] =
	{
		std::make_pair("GET", &Response::_execGet),
		std::make_pair("POST", &Response::_execGet),
		std::make_pair("DELETE", &Response::_execGet),
	};
	_methodsFunction.insert(methodsFunction, methodsFunction + SIZEOF(methodsFunction));
}

/* -------------------------------------------------------------------------- */
/*                                 constructor                                */
/* -------------------------------------------------------------------------- */

Response::Response(ConfigParser::Location &env, HTTPRequest &req, char **envSys)
: _env(env), _req(req), _envSys(envSys) 
{
	init_mimeTypes();
	init_methodsFunction();
}

/* -------------------------------------------------------------------------- */
/*                                   methods                                  */
/* -------------------------------------------------------------------------- */

std::vector<char>	Response::_getDefaultErrorPage(void)
{
	std::string	page("<center><h2> " + _code + " " + _status + " </h2></center>");
	std::vector<char>	res(page.begin(), page.end());
	return (res);
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

void	Response::_setError(std::string code)
{
	_code = code;
	_status = (*_env.nonUniqKey["return"][_code].begin());
	_data = _getDefaultErrorPage();
	_types = "text/html";
}

bool	Response::_checkFile(std::string filename)
{
	struct stat buf;
	
	stat(filename.c_str(), &buf);
	if (access(filename.c_str(), F_OK) != 0)
	{
		std::cout << "cc" << std::endl;
		_setError("404");
		return (true);
	}
	if (access(filename.c_str(), R_OK) != 0)
	{
		_setError("403");
		return (true);
	}
	if (S_ISDIR(buf.st_mode))
	{
		if ((*_env.uniqKey["auto_index"].begin()) == "on")
		{
			_code = "200";
			_status = (*_env.nonUniqKey["return"][_code].begin());
			_data = listingFile(filename);
			_types = "text/html";
		}
		else
			_setError("403");
		return (true);
	}
	return (false);
}

bool Response::_isCgiFile(std::string root)
{
	std::string	extension = root.substr(root.find_last_of("."));
	return (_env.nonUniqKey["cgi"].count(extension));
}

int	Response::_execCgi(std::string root)
{
	std::string extension = root.substr(root.find_last_of("."));
	std::string binCgi = (*_env.nonUniqKey["cgi"][extension].begin());
	int tabPipe[2];

	pipe(tabPipe);
	int pid = fork();
	if (pid == 0)
	{
		//tmp shit just to compile, we'll figure out a better solution later
		char **cmdArgs = new char*[3];

		cmdArgs[0] = new char[binCgi.size() + 1];
		binCgi.copy(cmdArgs[0], binCgi.size());
		cmdArgs[0][binCgi.size()] = '\0';

		cmdArgs[1] = new char[root.size() + 1];
		root.copy(cmdArgs[1], root.size());
		cmdArgs[1][root.size()] = '\0';

		cmdArgs[2] = NULL;

		dup2(tabPipe[1], STDOUT_FILENO);
		close(tabPipe[0]);
		close(tabPipe[1]);
		execve(binCgi.c_str(), cmdArgs, _envSys);
		delete []cmdArgs;
		perror("exec fail");
		exit(EXIT_FAILURE);
		std::cout << "c" << std::endl;
	}
	else
	{
		close(tabPipe[1]);
		std::cout << "debut wait" << std::endl;
		wait(NULL);
		std::cout << "fin wait" << std::endl;
	}
	return (tabPipe[0]);
}

		
void	Response::_execGet(void) {
	std::string root = _req.getData()["target"][0].erase(0, (*(_env.uniqKey["_rootToDel_"].begin())).length());
	root = *(_env.uniqKey["root"].begin()) + root;

	_setType(root);
	if (_checkFile(root))
		return ;
	std::ifstream file;
	if (_isCgiFile(root))
	{
		int pipefd = _execCgi(root);
		_readPipe(pipefd);
		_types = "text/html";
		return ;
	}
	file.open(root.c_str(), std::ios::binary);
	_readFile(file);
}

void	Response::_readPipe(int pipeToRead)
{
	char c;
	std::cout << "avant while" << std::endl;
	while (read(pipeToRead, &c, 1))
	{
		_data.push_back(c);
	}
	std::cout << "apres while" << std::endl;
	close(pipeToRead);
}

void	Response::_readFile(std::ifstream &file)
{
	while (file) 
	{
		char c;
		file.get(c);
		_data.push_back(c);
	}
	file.close();
}


void	Response::execute() {
	std::string method = _req.getData()["methods"][0];
	// tmp shit lol
	if (std::find(_env.uniqKey["allow_methods"].begin(), _env.uniqKey["allow_methods"].end(), method) != _env.uniqKey["allow_methods"].end())
	{
		(this->*(_methodsFunction.find(method)->second))();
		return ;	
	}
	_setError("405");
	return ;
}

void	Response::sendData(int clientFd)
{
	if (send(clientFd, _data.data(), _data.size(), 0) == -1)
        perror("send error:");
}

bool	Response::_isBinaryFile(std::string filePath)
{
	int	character;
	
	std::ifstream file(filePath.c_str());
	while ((character = file.get()) != EOF && character <= 127)
		;
	if (character == EOF)
		return (false);
	return (true);
}

void	Response::_setType(std::string url)
{
	std::string	extension = url.substr(url.find_last_of("."));
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

void	Response::constructData(void)
{
	HTTPRequest::request_type reqData = _req.getData();
	
	_header = reqData["httpVersion"][0] + " " + _code + " " + _status + "\r\n"
        + "Date: " + _getDate() + "\r\n"
        + "Server: webserv/1.0" + "\r\n"
        + "Content-Length: " + to_string(_data.size()) + "\r\n"
        + "Content-Type: " + _types + "\r\n"
        + "Connection: Closed" + "\r\n"
        + "\n";

	_data.insert(_data.begin(), _header.begin(), _header.end());
}
