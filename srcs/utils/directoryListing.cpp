/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 22:46:12 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/19 12:40:54 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <vector>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#include <iostream>

std::vector<char>	createIndexDir(std::vector<std::string> listOfFiles, std::string path)
{
	std::string	html;
	html.append("<h1>Index of \"" + path + "\"</h1>");
	html.append("<ul>");
	if (*path.rbegin() != '/')
		path.push_back('/');
	for (std::vector<std::string>::iterator it = listOfFiles.begin(); it != listOfFiles.end(); it++)
		html.append("<li><a href=\"" + path + *it + "\">" + *it + "</a></li>");
	html.append("</ul>");
	return (std::vector<char> (html.begin(), html.end()));
}

std::pair<std::string, bool>	getIndex(std::vector<std::string> listOfFiles, std::vector<std::string>	listOfIndex)
{
	for (std::vector<std::string>::iterator it = listOfIndex.begin(); it != listOfIndex.end(); it++)
	{
		std::vector<std::string>::iterator findRes = std::find(listOfFiles.begin(), listOfFiles.end(), *it);
		if (findRes != listOfFiles.end())
		{
			std::cout << "getIndex test : " << std::endl;
			std::cout << *findRes << std::endl;
			return (std::make_pair(*findRes, true));
		}
	}
	return (std::make_pair("", false));
}

std::vector<std::string>	listingFile(std::string realPath)
{
	DIR							*d;
	struct dirent				*dir;
	std::vector<std::string>	listOfFiles;
	
	d = opendir(realPath.c_str());
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
			listOfFiles.push_back(dir->d_name);
		closedir(d);
	}
	return (listOfFiles);
}
