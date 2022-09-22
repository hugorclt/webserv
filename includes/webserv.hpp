/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 10:52:17 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/22 18:14:19 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>
#include <cstring>
#include <fcntl.h>
#include "Server.hpp"
#include <sys/types.h>
#include "IOpoll.hpp"
#include <vector>
#include <map>
#include <cstring>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define	TRUE 1
#define FALSE 0
#define PORT 8080
#define MAX_EVENTS 5

/* -------------------------------------------------------------------------- */
/*                                  protoype                                  */
/* -------------------------------------------------------------------------- */
char											**ft_split(char const *str, const char *sep);
void											ft_free(char **tab);
char											*ft_substr(char const *s, unsigned int start, size_t len);
int												ft_strlen(const char *str);
int												getKeyLen(char *str);
int												getTabLen(char **tab);
char											*ft_strndup(char *str, int n);
int												parseStartLine(char **tab, std::map<std::string, std::vector<std::string>> &map);
std::map<std::string, std::vector<std::string>>	createHttpRequest(const char	*buffer);
char											**ft_split_value(char *line);
int												parseHeaders(char **tab, std::map<std::string, std::vector<std::string>> &map);
char											**ft_split_value(char *line);
char											*ft_substr(char const *s, unsigned int start, size_t len);
void											print_tab(char **tab);
char											*ft_substr(char *s, unsigned int start, size_t len);
