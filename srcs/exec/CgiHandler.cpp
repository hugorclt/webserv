/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:39:45 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/06 15:27:17 by hrecolet         ###   ########.fr       */
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
#include <errno.h>

CgiHandler::CgiHandler(ConfigParser::Location &server, Request &req, std::string MIMEtype, std::string clientIp, char **env, Response &res)
: _server(server), _req(req), _type(MIMEtype), _clientIp(clientIp), _sysEnv(env), _res(res)
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
{
	_pathToFile = _root + _target;
	_pathToFile.erase(_pathToFile.begin());
}


void	CgiHandler::_setCgiPath(void)
{
	if (_server.nonUniqKey["cgi"][_pathToFile.substr(_pathToFile.rfind("."))][0][0] == '.')
	{
		_cgiPath = _server.nonUniqKey["cgi"][_pathToFile.substr(_pathToFile.rfind("."))][0];
		_cgiPath.erase(_cgiPath.begin());
	}
	else
		_cgiPath = _server.nonUniqKey["cgi"][_pathToFile.substr(_pathToFile.rfind("."))][0];
}

std::string CgiHandler::_constructQuery(std::vector<std::string> var)
{
    std::string res;
    
    for (std::vector<std::string>::iterator it = var.begin(); it != var.end(); it++)
        res += *it + ((it + 1 != var.end()) ? "&" : "");
    return (res);
}

std::string	CgiHandler::_findDirectory(std::string path)
{
	std::string dir;

	dir = path.substr(path.find_last_of("/", path.find_last_of("/") - 1), path.find_last_of("/")) + "/";
	return (dir);
}

std::string	CgiHandler::_getSysPath(void)
{
	int i = 0;
	
	for (; _sysEnv[i]; i++)
	{
		if (ft_strncmp(_sysEnv[i], "PATH", 4) == 0)
			break;
	}
	return ((_sysEnv[i]) ? std::string(_sysEnv[i]).substr(5) : "");
}

void    CgiHandler::_initEnv(void)
{
	Request::request_type	header = _req.getData();
    std::vector<char> 		body = _req.getBody();
	std::string cwdPath = _setCwd();
	
    _env.push_back("REDIRECT_STATUS=200");
    _env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("CONTEXT_DOCUMENT_ROOT=" + cwdPath + _cgiPath.substr(0, _cgiPath.find_last_of('/') + 1));
	_env.push_back("CONTEXT_PREFIX=" + _findDirectory(_cgiPath));
	_env.push_back("HTTP_ACCEPT=" + header["Accept"][0]);
	if (header.count("Accept-Encoding") && !header["Accept-Encoding"].empty())
		_env.push_back("HTTP_ACCEPT_ENCODING=" + header["Accept-Encoding"][0]);
	if (header.count("Accept-Language") && !header["Accept-Language"].empty())
		_env.push_back("HTTP_ACCEPT_LANGUAGE=" + header["Accept-Language"][0]);
	_env.push_back("HTTP_CONNECTION=" + header["Connection"][0]);
	_env.push_back("HTTP_HOST=" + header["Host"][0] + ":" + header["Host"][1]);
	_env.push_back("HTTP_USER_AGENT=" + header["User-Agent"][0]);
    _env.push_back("CONTENT_LENGTH=" + to_string(_req.getEnvVar().size()));
	_env.push_back("PATH=" + _getSysPath());
   	_env.push_back("QUERY_STRING=" + _constructQuery(_req.getVar()));
    _env.push_back("REMOTE_ADDR=" + _clientIp);
    _env.push_back("REQUEST_METHOD=" + _req.getMethod());
	_env.push_back("REQUEST_SCHEME=http");
	_env.push_back("REQUEST_URI=" + _cgiPath);
    _env.push_back("SCRIPT_FILENAME=" + cwdPath + _pathToFile);
	_env.push_back("SCRIPT_NAME=" + _cgiPath);
	_env.push_back("SERVER_NAME=" + header["Host"][0]);
	_env.push_back("SERVER_PORT=" + header["Host"][1]);
	_env.push_back("SERVER_PORT=" + _req.getVersion()); //bizarre
	_env.push_back("SERVER_SIGNATURE=\"\"");
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

void    CgiHandler::_writeToIn(int pipeIn)
{
    std::string    toWrite = _req.getEnvVar();
    
    write(pipeIn, toWrite.data(), toWrite.size());
}

int	CgiHandler::exec(void)
{
	int tabPipe[2];
	int	tabVar[2];

	pipe(tabPipe);
	pipe(tabVar);
	int pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		dup2(tabPipe[1], STDOUT_FILENO);
		dup2(tabVar[0], STDIN_FILENO);
        close(tabPipe[0]);
		close(tabPipe[1]);
		close(tabVar[0]);
		_pathToFile.erase(_pathToFile.begin());
		_cgiPath.erase(_cgiPath.begin());
        char **env = _convertVecToChar(_env);
		std::vector<std::string> tmp(1, _cgiPath);
		char **nnll = _convertVecToChar(tmp);
		execve(_cgiPath.c_str(), nnll, env);
		g_exit = 1;
		throw CgiHandlerError("fork crashed");
    }
	else
	{
		_writeToIn(tabVar[1]);
		close(tabVar[1]);
		close(tabPipe[1]);
		close(tabVar[0]);
	}
	_res.setData(_readPipe(tabPipe[0]));
	return (0);
}

char    **CgiHandler::_convertVecToChar(std::vector<std::string> &vec)
{
    char **res;
    res = new char *[vec.size() + 1]; // reflechir plus tard
    
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
        res[it - vec.begin()] = &(*it)[0];
    res[vec.size()] = NULL;
    return (res);
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
    return (res);
}
