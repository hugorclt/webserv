/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/12 11:47:09 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/* -------------------------------------------------------------------------- */
/*                                CheckFunction                               */
/* -------------------------------------------------------------------------- */

// bool	Config::_isValueEmpty(std::vector<std::string> &vec)
// {
// 	return (!vec.size());
// }

// bool	Config::_checkAutoIndex(std::vector<std::string> &vec)
// {
// 	return ((vec.size() == 1) && (vec[0] == "on" || vec[0] == "off"));
// }

//  bool	Config::_checkBodySize(std::vector<std::string> &vec)
// {
// 	int	nb;
	
// 	if (_isValueEmpty(vec) || vec.size() > 1)
// 		return (false);
// 	if (!isDigits(vec[0]))
// 		return (false);

// 	nb = atoi(vec[0].c_str());
// 	if (nb <= 0)
// 		return (false);
// 	return (true);
// }

// bool	Config::_checkPath(std::vector<std::string> &vec)
// {
// 	return (vec.size() == 1);
// }

// bool	Config::_checkListen(std::vector<std::string> &vec)
// {
// 	int							port;
// 	std::vector<std::string>	ip;
	
// 	if (vec.size() != 2)
// 		return (false);
// 	if (!isDigits(vec[0]))
// 		return (false);
// 	port = atoi(vec[0].c_str());
// 	if (port < 0 || port > MAX_PORT)
// 		return (false);
// 	ip = split(vec[1], ".");
// 	if (ip.size() != 4)
// 		return (false);
// 	for (std::vector<std::string>::iterator it = ip.begin(); it < ip.end(); it++)
// 	{
// 		if (atoi(it->c_str()) < 0 || atoi(it->c_str()) > 255)
// 			return (false);
// 	}
// 	return (true);
// }

// bool	Config::_isValidKey(std::string key)
// {
// 	for (int i = 0; i < N_OPT; i++)
// 	{
// 		if (_option[i].first == key)
// 			return (true);
// 	}
// 	return (false);
// }

// std::pair<std::string, bool(*)(std::vector<std::string> &)>	Config::_getOpt(std::string key)
// {
// 	int i = 0;
	
// 	while (i < N_OPT && _option[i].first != key) {
// 		i++;
// 	}
// 	return (_option[i]);
// }

// const std::pair<std::string, bool(*)(std::vector<std::string> &)>	Config::_option[N_OPT] {
// 	std::make_pair(std::string("listen"), &Config::_checkListen),
// 	std::make_pair(std::string("server_name"), &Config::_checkPath), 
// 	std::make_pair(std::string("root"), &Config::_checkPath),
// 	std::make_pair(std::string("index"), &Config::_checkPath),
// 	std::make_pair(std::string("error"), &Config::_checkPath),
// 	std::make_pair(std::string("body_size"), &Config::_checkBodySize),
// 	std::make_pair(std::string("auto_index"), &Config::_checkAutoIndex)
// };


// bool	Config::_checkAllValue(map_type	&serverConfig)
// {
// 	for (map_type::iterator it = serverConfig.begin(); it != serverConfig.end(); it++)
// 	{
// 		if (!(*_getOpt(it->first).second)(it->second))
// 		{
// 			return (false);
// 		}
// 	}
// 	return (true);
// }

// bool	Config::_checkIpHost(void) {
// 	for (data_type::iterator it = _data.begin(), last = --_data.end(); it != last; it++)
// 
// 		if ((*it)["listen"] == (*last)["listen"])
// 			return (false);
// 	}
// 	return (true);
// }

/* -------------------------------------------------------------------------- */
/*                                 Constructor                                */
/* -------------------------------------------------------------------------- */

bool	Config::_isServer(std::pair<std::string, std::vector<std::string>> pair, lineRange_type &lineRange, fileRange_type &fileRange)
{
	_skipLineEmpty(lineRange, fileRange);
	return (pair.first == "server" && !pair.second.size() && *lineRange.first == '{');
}

bool	Config::_isLocation(std::pair<std::string, std::vector<std::string>> pair, lineRange_type &lineRange, fileRange_type &fileRange)
{
	_skipLineEmpty(lineRange, fileRange);
	return (pair.first == "location" && pair.second.size() == 1 && *lineRange.first == '{');
}

std::pair<std::string, std::vector<std::string>>	Config::_getKeyValuePair(lineRange_type &lineRange)
{
	std::string	key = _getWordSkipSpace(lineRange);
	
	std::vector<std::string>	values;
	std::string word = _getWordSkipSpace(lineRange);
	while (!word.empty())
	{
		values.push_back(word);
		word = _getWordSkipSpace(lineRange);
	}
	return (std::make_pair(key, values));
}

ServerConfig::confType	Config::_createNewLocation(lineRange_type &lineRange, fileRange_type &fileRange)
{
	ServerConfig::confType	res;

	_skipLineEmpty(lineRange, fileRange);
	std::pair<std::string, std::vector<std::string>>	pair = _getKeyValuePair(lineRange);
	while (fileRange.first != fileRange.second && *lineRange.first != '}')
	{
		_skipLineEmpty(lineRange, fileRange);
		res.insert(pair);
		pair = _getKeyValuePair(lineRange);
	}
	if (!pair.first.empty())
		res.insert(pair);
	if (fileRange.first == fileRange.second)
		throw ParsingError("Unclosed Location");
	lineRange.first++;
	return (res);
}

ServerConfig	Config::_createNewServerConfig(lineRange_type &lineRange, fileRange_type &fileRange)
{
	ServerConfig	res;
	
	_skipLineEmpty(lineRange, fileRange);
	std::pair<std::string, std::vector<std::string>>	pair = _getKeyValuePair(lineRange);
	while (fileRange.first != fileRange.second && (*lineRange.first != '}'))
	{
		_skipLineEmpty(lineRange, fileRange);
		if (_isLocation(pair, lineRange, fileRange))
		{
			lineRange.first++;
			res.location.insert(std::make_pair(pair.second[0], _createNewLocation(lineRange, fileRange)));
			continue ;
		}
		res.conf.insert(pair);
		pair = _getKeyValuePair(lineRange);
	}
	if (!pair.first.empty())
		res.conf.insert(pair);
	if (fileRange.first == fileRange.second)
		throw ParsingError("Unclosed Server");
	lineRange.first++;
	return (res);
}


Config::Config(char *filename) {
	std::ifstream				input(filename);
	std::vector<std::string>	fullFile;

	for (std::string line; std::getline(input, line); ) {
		fullFile.push_back(line);
	}
	
	lineRange_type	lineRange(fullFile.begin()->begin(), fullFile.begin()->end());
	fileRange_type	fileRange(fullFile.begin(), fullFile.end());

	for (;fileRange.first != fileRange.second;)
	{
		_skipLineEmpty(lineRange, fileRange);
		if (_isServer(_getKeyValuePair(lineRange), lineRange, fileRange))
		{
			lineRange.first++;
			_skipLineEmpty(lineRange, fileRange);
			_data.push_back(_createNewServerConfig(lineRange, fileRange));
		}
	}

	std::cout << _data[0].conf["listen"][0] << std::endl;
	std::cout << _data[0].conf["listen"].size() << std::endl;
	std::cout << _data[0].location.begin()->first << std::endl;
	std::cout << _data[0].location.begin()->second["allow_methods"][0] << std::endl;
	std::cout << _data[0].location["/hugo"]["root"][0] << std::endl;
	std::cout << _data[0].conf["error_page_404"][0] << std::endl;
}

/* -------------------------------------------------------------------------- */
/*                               ParsingFunction                              */
/* -------------------------------------------------------------------------- */

void		Config::_skipSpace(lineRange_type &lineRange)
{
	while (lineRange.first != lineRange.second && (*lineRange.first == '\t' || *lineRange.first == ' ')) {
		lineRange.first++;
	}
	if (lineRange.first != lineRange.second && *lineRange.first == '#')
		lineRange.first = lineRange.second;
}

void	skipPoint(std::pair<std::string::iterator, std::string::iterator> &lineRange)
{
	while (lineRange.first != lineRange.second && *lineRange.first == ';')
		lineRange.first++;
}

void	Config::_skipLineEmpty(lineRange_type &lineRange, fileRange_type &fileRange)
{
	if (fileRange.first == fileRange.second)
		return ;
	skipPoint(lineRange);
	_skipSpace(lineRange);
	skipPoint(lineRange);
	while (fileRange.first != fileRange.second && lineRange.first == lineRange.second) {
		fileRange.first++;
		if (fileRange.first == fileRange.second)
			break ;
		lineRange.first = fileRange.first->begin();
		lineRange.second = fileRange.first->end();
		skipPoint(lineRange);
		_skipSpace(lineRange);
		skipPoint(lineRange);
	}
}

std::string	Config::_getWord(lineRange_type &lineRange)
{
	std::string::iterator	it = lineRange.first;
	
	while (it != lineRange.second && *it != ' ' && *it != '\t' && *it != '{' && *it != '}' && *it != '#' && *it != ';')
		it++;
	std::string res(lineRange.first, it);
	lineRange.first = it;
	return (res);
}

std::string	Config::_getWordSkipSpace(lineRange_type &lineRange)
{
	_skipSpace(lineRange);
	std::string word = _getWord(lineRange);
	_skipSpace(lineRange);
	return (word);
}

/* -------------------------------------------------------------------------- */
/*                                  accessor                                  */
/* -------------------------------------------------------------------------- */

Config::data_type	Config::getData(void) {
	return (_data);
}
