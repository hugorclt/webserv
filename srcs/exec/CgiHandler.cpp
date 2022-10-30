/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:39:45 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/30 11:20:28 by hrecolet         ###   ########.fr       */
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

CgiHandler::CgiHandler(Request &req, std::string path,std::string MIMEtype, std::string clientIp, std::vector<std::string> var, std::string root)
{
    this->_var = var;
    this->_initEnv(req, path, MIMEtype, clientIp, root);
}

std::string CgiHandler::_constructQuery(std::vector<std::string> var)
{
    std::string res;
    
    for (std::vector<std::string>::iterator it = var.begin(); it != var.end(); it++)
        res += *it + ((it + 1 != var.end()) ? "&" : "");
    return (res);
}

void    CgiHandler::_initEnv(Request &req, std::string path, std::string MIMEtype, std::string clientIp, std::string root)
{
    (void)clientIp;
    (void)MIMEtype;
    char    *buf;
    buf = getcwd(NULL, BUF_SIZE);
    std::vector<std::string>    envVar = req.getEnvVar();
	Request::request_type		header = req.getData();
    std::vector<char> body = req.getBody();
    
    if (!buf)
        throw std::bad_alloc();
    if (path[0] == '.')
        path.erase(path.begin());
    if (root[0] == '.')
        root.erase(root.begin());
    this->_env.push_back("REDIRECT_STATUS=200");
    this->_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    this->_env.push_back("REQUEST_METHOD=" + req.getMethod());
    this->_env.push_back("CONTENT_LENGTH=" + to_string(req.getBody().size()));
	if (header.find("Content-Type") != header.end())
    	this->_env.push_back("CONTENT_TYPE=" + header["Content-Type"][0]);
	else
	    this->_env.push_back("CONTENT_TYPE=" + MIMEtype);
    this->_env.push_back("SCRIPT_FILENAME=" + std::string(buf) + root);
    // this->_env.push_back("REMOTEaddr=" + clientIp);
    // this->_env.push_back("HTTP_REFERER=" + req.getTarget());
    // this->_env.push_back("PATH=" +  path);
    // this->_env.push_back("PATH_INFO=" + req.getTarget());
    // this->_env.push_back("PATH_TRANSLATED=" + std::string(buf) + path);
    // this->_env.push_back("QUERY_STRING=" + _constructQuery(req.getVar()));
    // this->_env.insert(_env.end(), envVar.begin(), envVar.end());
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

void    CgiHandler::_writeToIn(Request &req, int pipeIn)
{
    std::vector<std::string>    envVar = req.getEnvVar();
    
    for (std::vector<std::string>::iterator it = envVar.begin(); it != envVar.end(); it++)
    {
		// dirty lovely ternary shit TERNARY MARRY ME !
        std::string toWrite(*it + ((it + 1 != envVar.end()) ? "&" : ""));
        write(pipeIn, toWrite.data(), toWrite.size());
    }
}

std::vector<char>	CgiHandler::exec(Request &req, std::string root, std::string binCgi)
{
	std::string extension = root.substr(root.find_last_of("."));
	int tabPipe[2];
	int	tabVar[2];

	pipe(tabPipe);
	pipe(tabVar);
    _writeToIn(req, tabVar[1]);
	close(tabVar[1]);
	int pid = fork();
	if (pid == 0)
	{
		dup2(tabPipe[1], STDOUT_FILENO);
		dup2(tabVar[0], STDIN_FILENO);
        close(tabPipe[0]);
		close(tabPipe[1]);
		close(tabVar[0]);
        _var.insert(_var.begin(), root);
        _var.insert(_var.begin(), binCgi);
        char **var = _convertVecToChar(_var);
        char **env = _convertVecToChar(_env);
		execve(binCgi.c_str(), var, env);
        delete []var;
        delete []env;
		perror("exec fail");
		exit(EXIT_FAILURE);
    }
	else
	{
		close(tabPipe[1]);
		close(tabVar[0]);
		wait(NULL);
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
