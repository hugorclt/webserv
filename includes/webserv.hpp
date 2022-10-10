/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 10:52:17 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/10 15:32:57 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <vector>
#include <map>
#include <cstring>
#include <arpa/inet.h>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cstdbool>


#include "Server.hpp"
#include "ServerList.hpp"
#include "IOpoll.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "../Parsing/Config.hpp"

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define	TRUE 1
#define FALSE 0
#define PORT 8080
#define MAX_EVENTS 5

/* -------------------------------------------------------------------------- */
/*                                  protoype                                  */
/* -------------------------------------------------------------------------- */
std::vector<std::string>						split(std::string s, std::string delimiter);
int												parseStartLine(char **tab, std::map<std::string, std::vector<std::string>> &map);
std::map<std::string, std::vector<std::string>>	createHttpRequest(std::string req);
int												parseHeaders(char **tab, std::map<std::string, std::vector<std::string>> &map);
void											print_tab(std::vector<std::string> tab);
std::vector<std::string> 						split(std::string s, char delimiter, int start);
int												countLenServer(std::istream &file);
int												ft_parse_server(std::istream &file, std::map<int, std::map<std::string, std::string>> &configFile);
std::string										getDate(void);
int                                         	execRequest(HTTPRequest	&req, HTTPResponse &res, std::string root);
std::string                                     to_string(int nb);
std::map<std::string, std::vector<std::string>>	parseOneServ(std::string::iterator it_str, std::vector<std::string>::iterator &it, std::vector<std::string> fullFile);
void											skipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd);
void											skipLineEmpty(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end);
std::string										getWordSkipLine(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end);
std::string										getWordSkipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd);
