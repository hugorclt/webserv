/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/12 11:45:11 by hrecolet         ###   ########.fr       */
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

bool	Config::_isServer(std::pair<std::string, std::vector<std::string>> pair, RangeIterator<std::string::iterator> strIt, RangeIterator<std::vector<std::string>::iterator> fileIt)
{
	_skipLineEmpty(strIt, fileIt);
	return (pair.first == "server" && !pair.second.size() && *strIt.first == '{');
}

bool	Config::_isLocation(std::pair<std::string, std::vector<std::string>> pair, RangeIterator<std::string::iterator> strIt, RangeIterator<std::vector<std::string>::iterator> fileIt)
{
	_skipLineEmpty(strIt, fileIt);
	return (pair.first == "location" && pair.second.size() == 1 && *strIt.first == '{');
}

std::pair<std::string, std::vector<std::string>>	Config::_getKeyValuePair(RangeIterator<std::string::iterator> strIt)
{
	std::string	key = _getWordSkipSpace(strIt);
	
	std::vector<std::string>	values;
	std::string word = _getWordSkipSpace(strIt);
	while (!word.empty())
	{
		values.push_back(word);
		word = _getWordSkipSpace(strIt);
	}
	return (std::make_pair(key, values));
}

ServerConfig::confType	Config::_createNewLocation(RangeIterator<std::string::iterator> strIt, RangeIterator<std::vector<std::string>::iterator> fileIt)
{
	ServerConfig::confType	res;

	_skipLineEmpty(strIt, fileIt);
	std::pair<std::string, std::vector<std::string>>	pair = _getKeyValuePair(strIt);
	while (fileIt.first != fileIt.second && (*strIt.first != '}'))
	{
		_skipLineEmpty(strIt, fileIt);
		res.insert(pair);
		pair = _getKeyValuePair(strIt);
	}
	return (res);
}

ServerConfig	Config::_createNewServerConfig(RangeIterator<std::string::iterator> strIt, RangeIterator<std::vector<std::string>::iterator> fileIt)
{
	ServerConfig	res;
	
	_skipLineEmpty(strIt, fileIt);
	std::pair<std::string, std::vector<std::string>>	pair = _getKeyValuePair(strIt);
	while (fileIt.first != fileIt.second && (*strIt.first != '}'))
	{
		_skipLineEmpty(strIt, fileIt);
		if (_isLocation(pair, strIt, fileIt))
		{
			strIt.first++;
			res.location.insert(std::make_pair(pair.second[0], _createNewLocation(strIt, fileIt)));
			continue ;
		}
		res.conf.insert(pair);
		pair = _getKeyValuePair(strIt);
	}
	return (res);
}


Config::Config(char *filename) {
	std::ifstream				input(filename);
	std::vector<std::string>	fullFile;

	for (std::string line; std::getline(input, line); ) {
		fullFile.push_back(line);
	}
	
	std::vector<std::string>::iterator itFile = fullFile.begin();
	std::vector<std::string>::iterator iteFile = fullFile.end();
	RangeIterator<std::vector<std::string>::iterator> fileIt(itFile, iteFile);
	std::string::iterator itStr = itFile->begin();
	std::string::iterator iteStr = iteFile->end();
	RangeIterator<std::string::iterator> strIt(itStr, iteStr);

	for (;fileIt.first < fileIt.second;)
	{
		strIt.first = fileIt.first->begin();
		strIt.second = fileIt.first->end();
		_skipLineEmpty(strIt, fileIt);
		if (_isServer(_getKeyValuePair(strIt), strIt, fileIt))
		{
			strIt.first++;
			_skipLineEmpty( strIt, fileIt);
			_data.push_back(_createNewServerConfig( strIt, fileIt));
		}
		break ;
		std::cout << "end for" << std::endl;
	}
	std::cout << "on sort du for" << std::endl;

	std::cout << _data[0].conf["listen"][0] << std::endl;
	std::cout << _data[0].location.begin()->first << std::endl;
	std::cout << _data[0].location.begin()->second["allow_methods"][0] << std::endl;



	
	// 	std::string word = _getWordSkipLine(itStr, itStrEnd, first, second);
	// 	if (word == "server")
	// 	{
	// 		if (_getWordSkipLine(itStr, itStrEnd, first, second) == "{")
	// 		{
	// 			map_type	serverConfig = _parseOneServ(itStr, itStrEnd, first, second);
	// 			if (!_checkAllValue(serverConfig))
	// 				throw ParsingError("error: parsing: value not viable in config file");
	// 			_data.push_back(serverConfig);
	// 			if (!_checkIpHost())
	// 				throw ParsingError("error: parsing: same address ip and host in config files");
	// 		}
	// 		else
	// 			throw ParsingError("error: parsing: '}' not closed");
	// 	else if (!word.empty())
	// 		throw ParsingError("error: parsing: no \"server\"");
}

	
	
	// int i = 1;
	// for (data_type::iterator it = _data.begin(); it < _data.end(); it++) {
	// 	std::cout << "Server :" << i << std::endl;
	// 	printMap(*it);
	// 	std::cout << std::endl;
	// 	i++;
	// }

/* -------------------------------------------------------------------------- */
/*                               ParsingFunction                              */
/* -------------------------------------------------------------------------- */

void		Config::_skipSpace(RangeIterator<std::string::iterator> strIt)
{
	while (strIt.first != strIt.second && (*strIt.first == '\t' || *strIt.first == ' ')) {
		strIt.first++;
	}
}

void	Config::_skipLineEmpty(RangeIterator<std::string::iterator> &strIt, RangeIterator<std::vector<std::string>::iterator> &fileIt)
{
	if (fileIt.first == fileIt.second)
		return ;
	_skipSpace(strIt);
	while (fileIt.first != fileIt.second && strIt.first == strIt.second) {
		fileIt.first++;
		if (fileIt.first == fileIt.second)
			break ;
		strIt.first = fileIt.first->begin();
		strIt.second = fileIt.first->end();
		_skipSpace(strIt);
	}
}

std::string	Config::_getWord(RangeIterator<std::string::iterator> strIt)
{
	std::string::iterator	it = strIt.first;
	
	while (it != strIt.second && *it != ' ' && *it != '\t' && *it != '{' && *it != '}')
		it++;
	std::string res(strIt.first, it);
	strIt.first = it;
	return (res);
}

std::string	Config::_getWordSkipSpace(RangeIterator<std::string::iterator> strIt)
{
	_skipSpace(strIt);
	std::string word = _getWord(strIt);
	_skipSpace(strIt);
	return (word);
}

// Config::map_type	Config::_parseOneServ(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &last)
// {
// 	std::map<std::string, std::vector<std::string>>	res;
// 	std::string word;
	
// 	while (first != last && *itStrBegin != '}')
// 	{
// 		std::string	key = _getWordSkipSpace(itStrBegin, itStrEnd);
// 		word = key;
// 		if ((key == "}" || key.empty()))
// 			break ;
// 		if (key == "location")
// 		{
			
// 		}
// 		if (!_isValidKey(key))
// 			throw ParsingError("error: parsing: not a valid key in config file");
// 		std::vector<std::string>	value;
// 		while (itStrBegin != itStrEnd)
// 		{
// 			std::string wordValue = _getWordSkipSpace(itStrBegin, itStrEnd);
// 			word = wordValue;
// 			if (word == "}")
// 				break ;
// 			value.push_back(wordValue);
// 		}
// 		if (!res.insert(std::make_pair(key, value)).second)
// 			throw ParsingError("error: parsing: doublon in config file");
// 		if (word == "}")
// 			break ;
// 		_skipLineEmpty(itStrBegin, itStrEnd, first, last);
// 		if (first == last)
// 			break ;
// 		itStrBegin = first->begin();
// 		itStrEnd = first->end();

// 	}
// 	if (word == "}")
// 		return (res);
// 	else
// 		throw ParsingError("error: parsing: '}' not closed");

// }

/* -------------------------------------------------------------------------- */
/*                                  accessor                                  */
/* -------------------------------------------------------------------------- */

Config::data_type	Config::getData(void) {
	return (_data);
}
