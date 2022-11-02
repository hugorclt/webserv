/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:39:45 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/02 11:54:16 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>

CgiHandler::CgiHandler(ConfigParser::Location &server, Request &req, std::string MIMEtype, std::string clientIp, char **env)
: _server(server), _req(req), _type(MIMEtype), _clientIp(clientIp), _sysEnv(env)
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
	std::string res(_sysEnv[i]);
	return (res.substr(5, res.size() - 5));
}

void    CgiHandler::_initEnv(void)
{
    char    				*buf;
	Request::request_type	header = _req.getData();
    std::vector<char> 		body = _req.getBody();
    
    buf = getcwd(NULL, BUF_SIZE);
    if (!buf)
		throw std::bad_alloc();
	std::string cwdPath(buf);
	
    _env.push_back("REDIRECT_STATUS=200");
    _env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	// _env.push_back("CONTEXT_DOCUMENT_ROOT=" + cwdPath + _cgiPath.substr(0, _cgiPath.find_last_of('/') + 1));
	// _env.push_back("CONTEXT_PREFIX=" + _findDirectory(_cgiPath));
	//DOCUMENT ROOT ?
	// _env.push_back("HTTP_ACCEPT=" + header["Accept"][0]);
	// _env.push_back("HTTP_ACCEPT_ENCODING=" + header["Accept-Encoding"][0]);
	// _env.push_back("HTTP_ACCEPT_LANGUAGE=" + header["Accept-Language"][0]);
	// _env.push_back("HTTP_CONNECTION=" + header["Connection"][0]);
	// _env.push_back("HTTP_HOST=" + header["Host"][0] + ":" + header["Host"][1]);
	//HTTP_IF_MODFIED_SINCE ?
	//HTTP_IF_NONE_MATCH ?
	// _env.push_back("HTTP_SEC_FETCH_DEST=" + header["Sec-Fetch-Dest"][0]);
	// _env.push_back("HTTP_SEC_FETCH_SITE=" + header["Sec-Fetch-Site"][0]);
	// _env.push_back("HTTP_SEC_FETCH_MODE=" + header["Sec-Fetch-Mode"][0]);
	// _env.push_back("HTTP_SEC_FETCH_USER=" + header["Sec-Fetch-User"][0]);
	// _env.push_back("HTTP_USER_AGENT=" + header["User-Agent"][0]);
    _env.push_back("CONTENT_LENGTH=" + to_string(_req.getEnvVar().size()));
	// _env.push_back("PATH=" + _getSysPath());
   	// _env.push_back("QUERY_STRING=" + _constructQuery(_req.getVar()));
    // _env.push_back("REMOTE_ADDR=" + _clientIp);
	// // REMOTE_PORT ?
    _env.push_back("REQUEST_METHOD=" + _req.getMethod());
	// _env.push_back("REQUEST_SCHEME=http");
	// _env.push_back("REQUEST_URI=" + _cgiPath);
    // _env.push_back("SCRIPT_FILENAME=" + cwdPath + _pathToFile);
	// _env.push_back("SCRIPT_NAME=" + _cgiPath);
	// SERVER_ADDR
	// _env.push_back("SERVER_NAME=" + header["Host"][0]);
	// _env.push_back("SERVER_PORT=" + header["Host"][1]);
	// _env.push_back("SERVER_PORT=" + _req.getVersion());
	// _env.push_back("SERVER_SIGNATURE=\"\"");
	// _env.push_back("SERVER_SOFTWARE=catzGang Web Server");
	if (header.find("Content-Type") != header.end())
    	_env.push_back("CONTENT_TYPE=" + header["Content-Type"][0]);
	else
	    _env.push_back("CONTENT_TYPE=" + _type);
}


void    ft_print_tab(char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        std::cerr << env[i] << std::endl;
        i++;
    }
}

void    CgiHandler::_writeToIn(int pipeIn)
{
    std::string    toWrite = _req.getEnvVar();
    
	std::cerr << toWrite << std::endl;
    write(pipeIn, toWrite.data(), toWrite.size());
}

std::vector<char>	CgiHandler::exec(void)
{
	int tabPipe[2];
	int	tabVar[2];

	pipe(tabPipe);
	pipe(tabVar);
    _writeToIn(tabVar[1]);
	close(tabVar[1]);
	int pid = fork();
	if (pid == 0)
	{
		dup2(tabPipe[1], STDOUT_FILENO);
		dup2(tabVar[0], STDIN_FILENO);
        close(tabPipe[0]);
		close(tabPipe[1]);
		close(tabVar[0]);
		_pathToFile.erase(_pathToFile.begin());
		_cgiPath.erase(_cgiPath.begin());
        _var.insert(_var.begin(), _pathToFile);
        _var.insert(_var.begin(), _cgiPath);
        char **var = _convertVecToChar(_var);
        char **env = _convertVecToChar(_env);
		ft_print_tab(env);
		std::cerr << "------argv-----" << std::endl;
		ft_print_tab(var);
		execve(_cgiPath.c_str(), var, env);
        delete []var;
        delete []env;
		perror("exec fail");
		exit(EXIT_FAILURE);
    }
	else
	{
		close(tabPipe[1]);
		close(tabVar[0]);
		std::cout << "oui je boucvle inf" << std::endl;
		wait(NULL);
		std::cout << "no boucle inf" << std::endl;
	}
	return (_readPipe(tabPipe[0]));
}

char    **CgiHandler::_convertVecToChar(std::vector<std::string> &vec)
{
    char **res;
    res = new char *[vec.size() + 1]; // reflechir plus tard
    
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        res[it - vec.begin()] = &(*it)[0];
    }
    res[vec.size()] = NULL;
    return (res);
}

std::vector<char> 	CgiHandler::_readPipe(int pipeToRead)
{
    std::vector<char>   res;
	char c;
	while (read(pipeToRead, &c, 1))
		res.push_back(c);
	close(pipeToRead);
    return (res);
}
