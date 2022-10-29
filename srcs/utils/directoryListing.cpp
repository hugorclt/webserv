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

std::vector<std::string>	getDir(std::string path)
{
	DIR							*dpdf;
	struct dirent 				*epdf;
	std::vector<std::string>	res;

	dpdf = opendir(path.c_str());
	if (dpdf)
	{
		while ((epdf = readdir(dpdf)))
			res.push_back(epdf->d_name);
	}
	closedir(dpdf);
	return (res);
}

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
		
	std::vector<char>	res(html.begin(), html.end());
	return (res);
}

std::vector<char>	listingFile(std::string realPath, std::string urlPath)
{
	DIR							*d;
	struct dirent				*dir;
	std::vector<std::string>	listOfFiles;
	std::vector<char>			res;

	
	d = opendir(realPath.c_str());
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			listOfFiles.push_back(dir->d_name);
		}
		closedir(d);
	}
	return(createIndexDir(listOfFiles, urlPath));
}
