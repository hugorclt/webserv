/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:39:45 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/08 03:29:10 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"
#include "utils.hpp"
#include "main.hpp"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <algorithm>
#include <errno.h>

CgiHandler::CgiHandler(ConfigParser::Location &server, Request &req, std::string MIMEtype, std::string clientIp, char **env, Response &res)
: _server(server), _req(req), _type(MIMEtype), _clientIp(clientIp), _sysEnv(env), _res(res), _contentType(""), _code("")
{
	_setTarget();
	_setRoot();
	_setPathToFile();
    _setCgiPath();
	_initEnv();
}

void	CgiHandler::_setTarget(void)
{
	_target = _req.getTarget().erase(0, _server.uniqKey["_rootToDel_"][0].length());
}

void CgiHandler::_setRoot(void)
{
	_root = _server.uniqKey["root"][0];
}

void	CgiHandler::_setPathToFile(void)
{ _pathToFile = _root + _target; }

void	CgiHandler::_setCgiPath(void)
{ _cgiPath = _server.nonUniqKey["cgi"][_pathToFile.substr(_pathToFile.rfind("."))][0]; }

std::string CgiHandler::_constructQuery(std::vector<std::string> var)
{
    std::string res;
    
    for (std::vector<std::string>::iterator it = var.begin(); it != var.end(); it++)
        res += *it + ((it + 1 != var.end()) ? "&" : "");
    return (res);
}

std::string	CgiHandler::_getSysPath(void)
{
	int i = 0;
	
	if (!_sysEnv)
		return ("");
	for (; _sysEnv[i]; i++)
	{
		if (ft_strncmp(_sysEnv[i], "PATH", 4) == 0)
			break;
	}
	return ((_sysEnv[i]) ? std::string(_sysEnv[i]).substr(5) : "");
}

void    CgiHandler::_initEnv(void)
{
    std::vector<char> 		body = _req.getBody();
	std::string cwdPath = _setCwd();
	
    _env.push_back("REDIRECT_STATUS=200");
    _env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	if (_cgiPath[0] == '/')
		_env.push_back("CONTEXT_DOCUMENT_ROOT=" + _cgiPath.substr(0, _cgiPath.rfind('/') + 1));
	else if (_cgiPath[0] == '.' && _cgiPath[1] == '/')
		_env.push_back("CONTEXT_DOCUMENT_ROOT=" + cwdPath + "/" + _cgiPath.substr(2, _cgiPath.rfind('/') - 1));
	else
		_env.push_back("CONTEXT_DOCUMENT_ROOT=" + cwdPath + "/" + _cgiPath.substr(0, _cgiPath.rfind('/') + 1));
	Request::request_type	header = _req.getData();
	if (header.count("Accept") && !header["Accept"].empty())
		_env.push_back("HTTP_ACCEPT=" + header["Accept"][0]);
	if (header.count("Accept-Encoding") && !header["Accept-Encoding"].empty())
		_env.push_back("HTTP_ACCEPT_ENCODING=" + header["Accept-Encoding"][0]);
	if (header.count("Accept-Language") && !header["Accept-Language"].empty())
		_env.push_back("HTTP_ACCEPT_LANGUAGE=" + header["Accept-Language"][0]);
	_env.push_back("HTTP_HOST=" + header["Host"][0] + ":" + header["Host"][1]);
	if (header.count("User-Agent") && !header["User-Agent"].empty())
		_env.push_back("HTTP_USER_AGENT=" + header["User-Agent"][0]);
    _env.push_back("CONTENT_LENGTH=" + to_string(_req.getEnvVar().size()));
	_env.push_back("PATH=" + _getSysPath());
   	_env.push_back("QUERY_STRING=" + _constructQuery(_req.getVar()));
    _env.push_back("REMOTE_ADDR=" + _clientIp);
    _env.push_back("REQUEST_METHOD=" + _req.getMethod());
	_env.push_back("REQUEST_SCHEME=http");
	_env.push_back("REQUEST_URI=" + _cgiPath);
	if (_pathToFile[0] == '/')
    	_env.push_back("SCRIPT_FILENAME=" + _pathToFile);
	else if (_pathToFile[0] == '.' && _pathToFile[1] == '/')
		_env.push_back("SCRIPT_FILENAME=" + cwdPath + _pathToFile.substr(1));
	else
		_env.push_back("SCRIPT_FILENAME=" + cwdPath + "/" + _pathToFile);
	_env.push_back("SCRIPT_NAME=" + _cgiPath);
	_env.push_back("SERVER_NAME=" + header["Host"][0]);
	_env.push_back("SERVER_PORT=" + header["Host"][1]);
	_env.push_back("SERVER_PORT=" + _req.getVersion());
	_env.push_back("SERVER_SIGNATURE=\"\"");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("SERVER_SOFTWARE=catzGang Web Server");
	if (header.find("Content-Type") != header.end())
    	_env.push_back("CONTENT_TYPE=" + header["Content-Type"][0]);
	else
	    _env.push_back("CONTENT_TYPE=" + _type);
}

std::string	CgiHandler::_setCwd(void)
{
	int		i = 1;
	char	*buf;

	while (i)
	{
		buf = new char[i];
		if (getcwd(buf, i))
			break;
		if (errno != ERANGE)
			throw CgiHandler::CgiHandlerError("getcwd failure");
		delete [] buf;
		buf = NULL;
		i++;
	}
	std::string res(buf);
	delete [] buf;
	return (res);
}

int    CgiHandler::_writeToIn(int pipeIn)
{
    std::string    toWrite = _req.getEnvVar();
    
    return (write(pipeIn, toWrite.data(), toWrite.size()));
}

int	CgiHandler::exec(void)
{
	int tabPipe[2];
	int	tabVar[2];

	if (pipe(tabPipe) == -1)
		throw CgiHandlerError("Error: pipe failed");
	if (pipe(tabVar) == -1)
	{
		close(tabPipe[0]);
		close(tabPipe[1]);
		throw CgiHandlerError("Error: pipe failed");
	}
	int pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(tabVar[1]);
		dup2(tabPipe[1], STDOUT_FILENO);
		dup2(tabVar[0], STDIN_FILENO);
        close(tabPipe[0]);
		close(tabPipe[1]);
		close(tabVar[0]);
		_pathToFile.erase(_pathToFile.begin());
        char **env = _convertVecToChar(_env);
		std::vector<std::string> tmp(1, _cgiPath);
		char **nnll = _convertVecToChar(tmp);
		execve(_cgiPath.c_str(), nnll, env);
		delete []nnll;
		delete []env;
		g_exit = 1;
		close(STDOUT_FILENO);
		throw CgiHandlerError("execve failed");
    }
	else
	{
		close(tabPipe[1]);
		close(tabVar[0]);
		if (_writeToIn(tabVar[1]) == -1)
		{
			close(tabVar[1]);
			close(tabPipe[0]);
			throw CgiHandlerError("Error: write");
		}
		close(tabVar[1]);
	}
	_res.setData(_readPipe(tabPipe[0]));
	return (0);
}

char    **CgiHandler::_convertVecToChar(std::vector<std::string> &vec)
{
    char **res;
    res = new char *[vec.size() + 1];
    
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
        res[it - vec.begin()] = &(*it)[0];
    res[vec.size()] = NULL;
    return (res);
}

void	CgiHandler::_parseFirstLine(std::vector<char> &body)
{
	std::cout << "start ParseFirstLine CGI HANDLER" << std::endl;
	if (body.empty())
		return ;
	std::vector<char>::iterator it = std::find(body.begin(), body.end(), '\n');
	std::string firstLine;
	std::vector<char>::iterator ite;
	if (it == body.end() && it != body.begin())
		return ;
	if (*(it - 1) == '\r')
	{
		if ((body.end() - it) < 2 || *(it + 1) != '\r' || *(it + 2) != '\n')
			return ;
		firstLine.assign(body.begin(), it - 1);
		ite = it + 3;
	}
	else
	{
		if ((body.end() - it) < 1 || *(it + 1) != '\n')
			return ;
		firstLine.assign(body.begin(), it);
		ite = it + 2;
	}
	std::cout << "firstLine : " << firstLine << std::endl;
	std::vector<std::string> vec = split_charset(firstLine, ";");
	std::map< std::string, std::vector<std::string> > map;
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		std::vector<std::string> tmp = split_charset(*it, " ");
		map.insert(std::make_pair(tmp[0], std::vector<std::string> (tmp.begin() + 1, tmp.end())));
	}
	if (map.count("Content-type:") && map["Content-type:"].size() == 1)
		_contentType = map["Content-type:"][0];
	if (map.count("Status:") && map["Status:"].size() == 1)
	{
		std::cout << "status trouver" << std::endl;	
		_code = map["Status:"][0];
	}
	if (!_code.empty() || !_contentType.empty())
		body.erase(body.begin(), ite);
}

std::vector<char> 	CgiHandler::_readPipe(int pipeToRead)
{
    std::vector<char>   res;
	int readState = 1;
	char buffer[4048];
	while ((readState = read(pipeToRead, buffer, 4048)) > 0)
		res.insert(res.end(), buffer, buffer + readState);
	close(pipeToRead);
	if (readState == -1)
		throw CgiHandlerError("read: error");
	wait(NULL);
	std::cerr << "cc1" << std::endl;
	_parseFirstLine(res);
    return (res);
}

std::string		CgiHandler::getContentType(void)
{
	return (_contentType);
}

std::string		CgiHandler::getCode(void)
{
	return (_code);
}
