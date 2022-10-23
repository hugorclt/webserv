/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:23:33 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/23 12:53:17 by hrecolet         ###   ########.fr       */
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
	{"GET", &Response::_execGet}, ///REMETTRE EXECGET !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{"POST", &Response::_execDel},
	{"DELETE", NULL}
};

/* -------------------------------------------------------------------------- */
/*                                 constructor                                */
/* -------------------------------------------------------------------------- */

Response::Response(ConfigParser::Location &env, Request &req, char **envSys) : _env(env), _req(req), _envSys(envSys) 
{
	std::vector<std::string> &var = req.getVar();
	
	for (std::vector<std::string>::iterator it = var.begin(); it != var.end(); it++)
	{
		_var.push_back(&((*it)[0]));
	}
}

/* -------------------------------------------------------------------------- */
/*                                   methods                                  */
/* -------------------------------------------------------------------------- */

std::vector<char>	Response::_getDefaultErrorPage(void)
{
	std::string	page(
	"<center>
		<h2> " + _code + " " + _status + " </h2>
	</center>"
	);
	std::vector<char>	res(page.begin(), page.end());
	return (res);
}

std::string	Response::_getDate(void) {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];
	
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (buffer, 80, "%a, %e %b %G %T GMT",timeinfo);
	return (std::string(buffer));
}

void	Response::_setError(std::string code)
{
	_code = code;
	_status = (*_env.nonUniqKey["return"][_code].begin());
	if (_checkFile(*(_env.nonUniqKey["error_page"][_code].begin()), 1) == false)
	{
		std::ifstream file(*(_env.nonUniqKey["error_page"][_code].begin()));
		_readFile(file);
	}
	else
		_data = _getDefaultErrorPage();
	_types = "text/html";
}

bool	Response::_checkFile(std::string filename, int isErrorFile)
{
	struct stat buf;
	
	stat(filename.c_str(), &buf);
	if (access(filename.c_str(), F_OK) != 0)
	{
		if (isErrorFile == 0)
			_setError("404");
		return (true);
	}
	if (access(filename.c_str(), R_OK) != 0)
	{
		if (isErrorFile == 0)
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
		{
			if (isErrorFile == 0)
				_setError("403");
		}
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
		_var.insert(_var.begin(), &root[0]);
		_var.insert(_var.begin() ,&binCgi[0]);\
		_var.push_back(NULL);
		dup2(tabPipe[1], STDOUT_FILENO);
		close(tabPipe[0]);
		close(tabPipe[1]);
		execve(binCgi.c_str(), _var.data(), _envSys);
		perror("exec fail");
		exit(EXIT_FAILURE);
		std::cout << "c" << std::endl;
	}
	else
	{
		close(tabPipe[1]);
		wait(NULL);
	}
	return (tabPipe[0]);
}

void	Response::_execDel(void)
{
	struct stat buf;
	std::string root = _req.getTarget().erase(0, (*(_env.uniqKey["_rootToDel_"].begin())).length());
	root = *(_env.uniqKey["root"].begin()) + root;
	
	stat(filename.c_str(), &buf);
	if (access(filename.c_str(), F_OK) != 0)
	{
		_setError("404");
		return ;
	}
	if (access(filename.c_str(), R_OK) != 0)
	{
		_setError("403");
		return ;
	}
	if (S_ISDIR(buf.st_mode))
	{
		_setError("403");
		return ;
	}
	unlink(root.c_str());
}

void	Response::_execGet(void) {
	std::string root = _req.getTarget().erase(0, (*(_env.uniqKey["_rootToDel_"].begin())).length());
	root = *(_env.uniqKey["root"].begin()) + root;

	_setType(root);
	if (_checkFile(root, 0))
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
	while (read(pipeToRead, &c, 1))
		_data.push_back(c);
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
	std::string method = _req.getMethod();
	
	if (_env.uniqKey["allow_methods"].count(method))
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
	
	std::ifstream file(filePath);
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
	_header = _req.getVersion() + " " + _code + " " + _status + "\r\n"
        + "Date: " + _getDate() + "\r\n"
        + "Server: webserv/1.0" + "\r\n"
        + "Content-Length: " + to_string(_data.size()) + "\r\n"
        + "Content-Type: " + _types + "\r\n"
        + "Connection: Closed" + "\r\n"
        + "\n";

	_data.insert(_data.begin(), _header.begin(), _header.end());
}