/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 10:52:17 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/19 13:13:36 by hrecolet         ###   ########.fr       */
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
#include <dirent.h>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <vector>
#include <map>
#include <set>
#include <cstring>
#include <arpa/inet.h>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cstdbool>
#include <exception>
#include <cerrno>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>

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
//int                                         	execRequest(HTTPRequest	&req, Response &res, std::string root);
std::string                                     to_string(int nb);
void											printMap(std::map<std::string, std::vector<std::string>> map);
bool											isDigits(std::string &digits);
std::vector<char>                               listingFile(std::string root);
