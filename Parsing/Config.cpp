/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 20:36:02 by hrecolet         ###   ########.fr       */
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

bool	Config::_isServer(std::pair<std::string, std::vector<std::string>> pair, std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &second)
{
	_skipLineEmpty(itStrBegin, itStrEnd, first, second);
	return (pair.first == "server" && !pair.second.size() && *itStrBegin == '{');
}

bool	Config::_isLocation(std::pair<std::string, std::vector<std::string>> pair, std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &second)
{
	_skipLineEmpty(itStrBegin, itStrEnd, first, second);
	return (pair.first == "location" && pair.second.size() == 1 && *itStrBegin == '{');
}

std::pair<std::string, std::vector<std::string>>	Config::_getKeyValuePair(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd)
{
	std::string	key = _getWordSkipSpace(itStrBegin, itStrEnd);
	
	std::vector<std::string>	values;
	std::string word = _getWordSkipSpace(itStrBegin, itStrEnd);
	while (!word.empty())
	{
		values.push_back(word);
		word = _getWordSkipSpace(itStrBegin, itStrEnd);
	}
	return (std::make_pair(key, values));
}

ServerConfig::confType	Config::_createNewLocation(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &second)
{
	ServerConfig::confType	res;

	_skipLineEmpty(itStrBegin, itStrEnd, first, second);
	std::pair<std::string, std::vector<std::string>>	pair = _getKeyValuePair(itStrBegin, itStrEnd);
	while (first != second && (*itStrBegin != '}'))
	{
		_skipLineEmpty(itStrBegin, itStrEnd, first, second);
		res.insert(pair);
		pair = _getKeyValuePair(itStrBegin, itStrEnd);
	}
	return (res);
}

ServerConfig	Config::_createNewServerConfig(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &second)
{
	ServerConfig	res;
	
	_skipLineEmpty(itStrBegin, itStrEnd, first, second);
	std::pair<std::string, std::vector<std::string>>	pair = _getKeyValuePair(itStrBegin, itStrEnd);
	while (first != second && (*itStrBegin != '}'))
	{
		_skipLineEmpty(itStrBegin, itStrEnd, first, second);
		if (_isLocation(pair, itStrBegin, itStrEnd, first, second))
		{
			itStrBegin++;
			res.location.insert(std::make_pair(pair.second[0], _createNewLocation(itStrBegin, itStrEnd, first, second)));
			continue ;
		}
		res.conf.insert(pair);
		pair = _getKeyValuePair(itStrBegin, itStrEnd);
	}
	return (res);
}


Config::Config(char *filename) {
	std::ifstream				input(filename);
	std::vector<std::string>	fullFile;

	for (std::string line; std::getline(input, line); ) {
		fullFile.push_back(line);
	}
	
	std::vector<std::string>::iterator first = fullFile.begin();
	std::string::iterator 				itStr = first->begin();
	std::string::iterator 				itStrEnd = first->end();
	std::vector<std::string>::iterator	second = fullFile.end();

	// _skipLineEmpty(itStr, itStrEnd, first, second);
	// std::pair<std::string, std::vector<std::string>>	p = _getKeyValuePair(itStr, itStrEnd);
	// _skipLineEmpty(itStr, itStrEnd, first, second);
	// std::cout << "key : " << p.first << std::endl
	// 		  << "value size " << p.second.size() << std::endl;
	// if (p.second.size())
	// 	std::cout << "value[0]" << p.second[0] << std::endl;

	// std::cout << ((p.first == "server" && !p.second.size() && *itStr == '{') ? "detection d'un server" : "error") << std::endl;
	

	for (;first < second;)
	{
		std::cout << "start for" << *itStr << " " << *first << std::endl;
		itStr = first->begin();
		itStrEnd = first->end();
		_skipLineEmpty(itStr, itStrEnd, first, second);
		if (_isServer(_getKeyValuePair(itStr, itStrEnd), itStr, itStrEnd, first, second))
		{
			itStr++;
			_skipLineEmpty(itStr, itStrEnd, first, second);
			_data.push_back(_createNewServerConfig(itStr, itStrEnd, first, second));
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

void		Config::_skipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd)
{
	while (itStrBegin != itStrEnd && (*itStrBegin == '\t' || *itStrBegin == ' ')) {
		itStrBegin++;
	}
}

void	Config::_skipLineEmpty(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end)
{
	if (first == end)
		return ;
	_skipSpace(itStrBegin, itStrEnd);
	while (first != end && itStrBegin == itStrEnd) {
		first++;
		if (first == end)
			break ;
		itStrBegin = first->begin();
		itStrEnd = first->end();
		_skipSpace(itStrBegin, itStrEnd);
	}
}

std::string	Config::_getWord(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd)
{
	std::string::iterator	it = itStrBegin;
	
	while (it != itStrEnd && *it != ' ' && *it != '\t' && *it != '{' && *it != '}')
		it++;
	std::string res(itStrBegin, it);
	itStrBegin = it;
	return (res);
}

std::string	Config::_getWordSkipLine(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end)
{
	_skipLineEmpty(itStrBegin, itStrEnd, first, end);
	std::string word = _getWord(itStrBegin, itStrEnd);
	_skipLineEmpty(itStrBegin, itStrEnd, first, end);
	return (word);
}

std::string	Config::_getWordSkipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd)
{
	_skipSpace(itStrBegin, itStrEnd);
	std::string word = _getWord(itStrBegin, itStrEnd);
	_skipSpace(itStrBegin, itStrEnd);
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
