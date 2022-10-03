/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 10:52:17 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/03 13:54:02 by hrecolet         ###   ########.fr       */
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
#include <sstream>


#include "Server.hpp"
#include "IOpoll.hpp"
#include "HTTPRequest.hpp"
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