/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/12 21:15:44 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

const std::string Config::_whitespacesSet = "\t ";
const std::string Config::_lineBreakSet = ";";
const std::string Config::_commentSet = "#";
const std::string Config::_scopeSet = "{}";

const std::pair<std::string, bool(*)(std::vector<std::string> &)>	Config::_serverKey[N_SERVER_KEY] {
	{"listen", &Config::_checkListen},
	{"server_name", &Config::_checkPath}, 
};

const std::pair<std::string, bool(*)(std::vector<std::string> &)>	Config::_nonUniqKey[N_NON_UNIQ_KEY] {
	{"cgi", &Config::_checkPath},
	{"error_page", &Config::_checkPath},
};

const std::pair<std::string, bool(*)(std::vector<std::string> &)>	Config::_uniqKey[N_UNIQ_KEY] {
	{"body_size", &Config::_checkPath},
	{"allow_methods", &Config::_checkPath},
	{"root", &Config::_checkPath},
	{"index", &Config::_checkPath},
	{"auto_index", &Config::_checkPath},
};

/* -------------------------------------------------------------------------- */
/*                                CheckFunction                               */
/* -------------------------------------------------------------------------- */

/*
bool	Config::_checkKeyConfServer(ServerConfig::confType &confServ)
{
	if (confServ.find("listen") == confServ.end())
		return (false);
	if (confServ.find("body_size") == confServ.end())
		return (false);

	//add loop throught the map and send all vector in the tab of pointer function _optionConf
	return (true);
}

bool	Config::_checkKeyLocation(ServerConfig::locationType &confLocation)
{
	if (confLocation.size() < 1)
		return (false);
	for (ServerConfig::locationType::iterator itLocations = confLocation.begin(); itLocations != confLocation.end(); itLocations++)
	{
		if (itLocations->second.find("root") == itLocations->second.end())
			return (false);
		//add loop throught the map and send all vector in the tab of pointer function _optionLocation

	}
	return (true);
}
*/

bool	Config::_checkAutoIndex(std::vector<std::string> &vec)
{
	return ((vec.size() == 1) && (vec[0] == "on" || vec[0] == "off"));
}

 bool	Config::_checkBodySize(std::vector<std::string> &vec)
{
	int	nb;
	
	if (vec.size() == 1)
		return (false);
	if (!isDigits(vec[0]))
		return (false);

	nb = atoi(vec[0].c_str());
	if (nb <= 0)
		return (false);
	return (true);
}

bool	Config::_checkPath(std::vector<std::string> &vec)
{
	return (vec.size() == 1);
}

bool	Config::_checkListen(std::vector<std::string> &vec)
{
	int							port;
	std::vector<std::string>	ip;
	
	if (vec.size() != 2)
		return (false);
	if (!isDigits(vec[0]))
		return (false);
	port = atoi(vec[0].c_str());
	if (port < 0 || port > MAX_PORT)
		return (false);
	ip = split(vec[1], ".");
	if (ip.size() != 4)
		return (false);
	for (std::vector<std::string>::iterator it = ip.begin(); it < ip.end(); it++)
	{
		if (atoi(it->c_str()) < 0 || atoi(it->c_str()) > 255)
			return (false);
	}
	return (true);
}

bool	Config::_isServerKey(const std::string &key)
{
	for (size_t i = 0; i < N_SERVER_KEY; i++)
	{
		if (_serverKey[i].first == key)
			return (true);
	}
	return (false);
}

bool	Config::_isUniqKey(const std::string &key)
{ 
	for (size_t i = 0; i < N_UNIQ_KEY; i++)
	{
		if (_uniqKey[i].first == key)
			return (true);
	}
	return (false);
}

bool	Config::_isNonUniqKey(const std::string &key)
{ 
	for (size_t i = 0; i < N_NON_UNIQ_KEY; i++)
	{
		if (_nonUniqKey[i].first == key)
			return (true);
	}
	return (false);
}

bool	Config::_isValidKey(const std::string &key)
{ return (_isUniqKey(key) || _isNonUniqKey(key)); }

// std::pair<std::string, bool(*)(std::vector<std::string> &)>	Config::_getOpt(std::string key)
// {
// 	int i = 0;
	
// 	while (i < N_OPT && _option[i].first != key) {
// 		i++;
// 	}
// 	return (_option[i]);
// }



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

static void	printVec(const std::vector<std::string> &vec)
{
	if (vec.empty())
		std::cout << "(NONE)";
	for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); it++)
		std::cout << " " << *it;
}

static void printMap(const std::map<std::string, std::vector<std::string>> &map, const std::string &indent)
{
	if (map.empty())
		std::cout << indent << "(NONE)" << std::endl;
	for (std::map<std::string, std::vector<std::string>>::const_iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << indent << "[\"" << it->first << "\"] :";
		printVec(it->second);
		std::cout << std::endl;
	}
}

void	Config::_printConfig(const data_type &data)
{
	if (data.empty())
	{
		std::cout << "(NONE)" << std::endl;
		return ;
	}
	for (data_type::const_iterator itServ = data.begin(); itServ != data.end(); itServ++)
	{
		std::string	indent("");
		std::cout << indent << "Server " << data.size() - (data.end() - itServ) << " :" << std::endl;
		std::cout << indent << '{' << std::endl;
		for (ServerConfig::locationType::const_iterator itLoc = itServ->location.begin(); itLoc != itServ->location.end(); itLoc++)
		{
			std::string indent("\t");
			std::cout << indent << "Location [\"" << itLoc->first << "\"] :" << std::endl;
			std::cout << indent << '{' << std::endl;
			indent += "\t";
			std::cout << indent << "UniqKey :" << std::endl;
			std::cout << indent << '{' << std::endl;
			printMap(itLoc->second.uniqKey, indent + "\t");
			std::cout << indent << '}' << std::endl
					  << std::endl;
			std::cout << indent << "NonUniqKey :" << std::endl;
			std::cout << indent << '{' << std::endl;
			if (itLoc->second.nonUniqKey.empty())
				std::cout << indent + "\t" << "(NONE)" << std::endl;
			for (LocationConfig::nonUniqKey_type::const_iterator it = itLoc->second.nonUniqKey.begin(); it != itLoc->second.nonUniqKey.end(); it++)
			{
				std::string indent("\t\t\t");
				std::cout << indent << it->first << " :" << std::endl;
				std::cout << indent << '[' << std::endl;
				printMap(it->second, indent + "\t");
				std::cout << indent << ']' << std::endl;
			}
			std::cout << indent << '}' << std::endl;
			indent = "\t";
			std::cout << indent << '}' << std::endl
				  	  << std::endl
					  << std::endl;
		}
		std::cout << indent << '}' << std::endl
				  << std::endl
				  << std::endl
				  << std::endl;
	}
}

Config::Config(char *filename) {
	std::ifstream				input(filename);
	std::vector<std::string>	fullFile;

	for (std::string line; std::getline(input, line); ) {
		fullFile.push_back(line);
	}
	
	lineRange_type	lineRange(fullFile.begin()->begin(), fullFile.begin()->end());
	fileRange_type	fileRange(fullFile.begin(), fullFile.end());

	try
	{
		for (;fileRange.first != fileRange.second;)
		{
			_goToNextWordInFile(lineRange, fileRange);
			if (_isServer(_getKeyValues(lineRange), lineRange, fileRange))
			{
				lineRange.first++;
				_goToNextWordInFile(lineRange, fileRange);
				_data.push_back(_createNewServerConfig(lineRange, fileRange));
			}
			else
				throw ParsingError("wrong Token Global Scope");
			_goToNextWordInFile(lineRange, fileRange);
		}
	}
	catch (ParsingError &e)
	{
		throw ParsingError(std::string(e.what()) + " :\n" + "line " + to_string(fullFile.size() - (fileRange.second - fileRange.first) + 1) + " : " + *fileRange.first);
	}
	_printConfig(_data);
}

/* -------------------------------------------------------------------------- */
/*                               ParsingFunction                              */
/* -------------------------------------------------------------------------- */

bool	Config::_isServer(keyValues_type keyValues, lineRange_type &lineRange, fileRange_type &fileRange)
{
	if (keyValues.first != "server" || !keyValues.second.empty())
		return (false);
	_goToNextWordInFile(lineRange, fileRange);
	return (*lineRange.first == '{');
}

bool	Config::_isLocation(keyValues_type keyValues, lineRange_type &lineRange, fileRange_type &fileRange)
{
	lineRange_type	lineRangeCpy = lineRange;
	fileRange_type	fileRangeCpy = fileRange;
	_goToNextWordInFile(lineRangeCpy, fileRangeCpy);
	return (keyValues.first == "location" && keyValues.second.size() == 1 && *lineRangeCpy.first == '{');
}

Config::keyValues_type	Config::_getKeyValues(lineRange_type &lineRange)
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

void			Config::_insertKeyValuesInLocation(LocationConfig &location, keyValues_type &keyValues)
{
	if (!_isValidKey(keyValues.first))
		throw ParsingError("Unregognized Key : " + keyValues.first);
	if (keyValues.second.empty())
		throw ParsingError("Key without options");
	if (_isUniqKey(keyValues.first) && !location.uniqKey.insert(keyValues).second)
		throw ParsingError("Key \"" + keyValues.first + "\" already exist");
	else if (_isNonUniqKey(keyValues.first) && !(location.nonUniqKey[keyValues.first].insert(std::make_pair(keyValues.second[0], std::vector<std::string> (keyValues.second.begin() + 1, keyValues.second.end())))).second)
		throw ParsingError("subKey \"" + keyValues.second[0] + "\" already exist");
}

LocationConfig	Config::_createNewLocation(lineRange_type &lineRange, fileRange_type &fileRange)
{
	LocationConfig	res;

	_goToNextWordInFile(lineRange, fileRange);
	keyValues_type	keyValues = _getKeyValues(lineRange);
	while (fileRange.first != fileRange.second && !keyValues.first.empty())
	{
		_insertKeyValuesInLocation(res, keyValues);
		_goToNextWordInFile(lineRange, fileRange);
		keyValues = _getKeyValues(lineRange);
	}
	if (*lineRange.first == '{')
		throw ParsingError("Wrong { token in Location");
	if (fileRange.first == fileRange.second)
		throw ParsingError("Unclosed Location");
	lineRange.first++;
	_goToNextWordInFile(lineRange, fileRange);
	return (res);
}

ServerConfig	Config::_createNewServerConfig(lineRange_type &lineRange, fileRange_type &fileRange)
{
	ServerConfig	res;
	LocationConfig	serverLocationConf;
	
	_goToNextWordInFile(lineRange, fileRange);
	keyValues_type	keyValues = _getKeyValues(lineRange);
	while (fileRange.first != fileRange.second && !keyValues.first.empty())
	{
		if (_isLocation(keyValues, lineRange, fileRange))
		{
			if (res.location.find(keyValues.second[0]) != res.location.end())
				throw ParsingError("Location duplication : \"" + keyValues.second[0] + "\"");
			_goToNextWordInFile(lineRange, fileRange); //skip the location
			lineRange.first++; //skip the location
			try { res.location.insert(std::make_pair(keyValues.second[0], _createNewLocation(lineRange, fileRange))); }
			catch (ParsingError &e) { throw ParsingError("location \"" + keyValues.second[0] + "\" : " + e.what()); }
		}
		else
		{
			if (!_isServerKey(keyValues.first))
				_insertKeyValuesInLocation(serverLocationConf, keyValues);
			//_insertKeyValuesInServer(res, keyValues);
		}
		_goToNextWordInFile(lineRange, fileRange);
		keyValues = _getKeyValues(lineRange);
	}
	if (*lineRange.first == '{')
		throw ParsingError("Wrong { token in Server (not related to a Location)");
	if (fileRange.first == fileRange.second)
		throw ParsingError("Unclosed Server");
	lineRange.first++;
	for (LocationConfig::uniqKey_type::iterator it = serverLocationConf.uniqKey.begin(); it != serverLocationConf.uniqKey.end(); it++)
		res.location["/"].uniqKey.insert(*it);
	for (LocationConfig::nonUniqKey_type::iterator it = serverLocationConf.nonUniqKey.begin(); it != serverLocationConf.nonUniqKey.end(); it++)
		for (LocationConfig::uniqKey_type::iterator nit = it->second.begin(); nit != it->second.end(); nit++)
			res.location["/"].nonUniqKey[it->first].insert(*nit);
	return (res);
}

void	Config::_skipCharset(lineRange_type &lineRange, const std::string &charset)
{ 
	while (lineRange.first != lineRange.second && charset.find(*lineRange.first) != std::string::npos)
		lineRange.first++;
	if (lineRange.first != lineRange.second && _commentSet.find(*lineRange.first) != std::string::npos)
		lineRange.first = lineRange.second;
}

void	Config::_goToNextWordInFile(lineRange_type &lineRange, fileRange_type &fileRange)
{
	if (fileRange.first == fileRange.second)
		return ;
	_skipCharset(lineRange, _whitespacesSet + _lineBreakSet);
	while (fileRange.first != fileRange.second && lineRange.first == lineRange.second) {
		fileRange.first++;
		if (fileRange.first == fileRange.second)
			break ;
		lineRange.first = fileRange.first->begin();
		lineRange.second = fileRange.first->end();
		_skipCharset(lineRange, _whitespacesSet + _lineBreakSet);
	}
}

std::string	Config::_getWord(lineRange_type &lineRange)
{
	std::string::iterator	it = lineRange.first;
	
	while (it != lineRange.second && (_whitespacesSet + _lineBreakSet + _commentSet + _scopeSet).find(*it) == std::string::npos)
		it++;
	std::string res(lineRange.first, it);
	lineRange.first = it;
	return (res);
}

std::string	Config::_getWordSkipSpace(lineRange_type &lineRange)
{
	_skipCharset(lineRange, _whitespacesSet);
	std::string word = _getWord(lineRange);
	_skipCharset(lineRange, _whitespacesSet);
	return (word);
}

/* -------------------------------------------------------------------------- */
/*                                  accessor                                  */
/* -------------------------------------------------------------------------- */

Config::data_type	Config::getData(void) {
	return (_data);
}
