/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 14:21:50 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/17 14:22:13 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <map>
#include <string>

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
