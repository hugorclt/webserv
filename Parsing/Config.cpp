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
#include <climits>

const std::string Config::_whitespacesSet = "\t ";
const std::string Config::_lineBreakSet = ";";
const std::string Config::_commentSet = "#";
const std::string Config::_scopeSet = "{}";

const std::map<std::string, bool(*)(std::vector<std::string> &)>	Config::_serverKey{
	{"listen", NULL},
	{"server_name", NULL}, 
};


const std::map<std::pair<std::string, Config::_KeyType>, std::pair<void(*)(std::vector<std::string> &), std::pair<int, std::set<std::string>>>>	Config::_keyParsingMap
{
	// if POSSIBLE PARAMS is empty it means thant it's checked in the FUNC
	// {{    KEY     , KEY_TYPE }, {FUNC, {MAX_PARAMS, {POSSIBLE PARAMS}}}}
	{{"body_size"    , UNIQ_KEY    }, {&_checkBodySize, {1, {                       }}}},
	{{"allow_methods", UNIQ_KEY    }, {NULL           , {3, {"GET", "POST", "DELETE"}}}},
	{{"root"         , UNIQ_KEY    }, {NULL           , {1, {                       }}}},
	{{"index"        , UNIQ_KEY    }, {NULL           , {1, {                       }}}},
	{{"auto_index"   , UNIQ_KEY    }, {NULL           , {1, {"on", "off"            }}}},
	{{"cgi"          , NON_UNIQ_KEY}, {&_checkCgi     , {1, {                       }}}},
	{{"error_page"   , NON_UNIQ_KEY}, {NULL           , {1, {"404", "403", "442"    }}}}
};

/* -------------------------------------------------------------------------- */
/*                                CheckFunction                               */
/* -------------------------------------------------------------------------- */

void	Config::_checkCgi(std::vector<std::string> &vec)
{
	if (vec[0].rfind('.') != 0)
		throw ParsingError("cgi SubKey should always contains one and no more '.' at start as it should be an ext");
	if (vec[0].size() < 2)
		throw ParsingError("cgi SubKey should have at least one char after the point");
}

void	Config::_checkBodySize(std::vector<std::string> &vec)
{
	if (!isDigits(vec[0]))
		throw ParsingError("body_size is a positive integer and should contains only numbers");
	if (atoi(vec[0].c_str()) <= 0)
		throw ParsingError("body_size overflow, max value : " + to_string(INT_MAX));
}

/* Saved for later help
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
*/

bool	Config::_isServerKey(const std::string &key)
{ return (_serverKey.find(key) != _serverKey.end()); }

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
	std::cout << "PrintConfig :" << std::endl << std::endl;
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
		std::cout << indent << '}' << std::endl << std::endl << std::endl << std::endl;
	}
}

Config::Config(char *filename) {
	std::ifstream				input(filename);
	std::vector<std::string>	fullFile;

	for (std::string line; std::getline(input, line); ) {
		fullFile.push_back(line);
	}
	
	if (fullFile.empty())
	{
		_printConfig(_data);
		return ;
	}

	lineRange_type	lineRange(fullFile.begin()->begin(), fullFile.begin()->end());
	fileRange_type	fileRange(fullFile.begin(), fullFile.end());

	try
	{
		while (fileRange.first != fileRange.second)
		{
			_goToNextWordInFile(lineRange, fileRange);
			if (_isServer(_getKeyValues(lineRange), lineRange, fileRange))
			{
				lineRange.first++;
				_goToNextWordInFile(lineRange, fileRange);
				try { _data.push_back(_createNewServerConfig(lineRange, fileRange)); }
				catch (ParsingError &e) { throw ParsingError("Server " + to_string(_data.size()) + " : " + e.what()); }
			}
			else if (fileRange.first != fileRange.second)
				throw ParsingError("wrong Token Global Scope");
			_goToNextWordInFile(lineRange, fileRange);
		}
	}
	catch (ParsingError &e)
	{
		throw ParsingError(std::string(e.what()) + " :\n"
		+ "line " + to_string(fullFile.size() - (fileRange.second - fileRange.first) + 1) + " : "
		+ *(fileRange.first - (fileRange.first == fileRange.second)));
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

void			Config::_insertKeyValuesInLocation(LocationConfig &location, keyValues_type &keyValues) // need optimization (synt)
{
	if (_keyParsingMap.count(std::make_pair(keyValues.first, UNIQ_KEY)))
	{
		const std::pair<void(*)(std::vector<std::string> &), std::pair<int, std::set<std::string>>>	tmp = _keyParsingMap.find(std::make_pair(keyValues.first, UNIQ_KEY))->second;
		if (keyValues.second.empty())
			throw ParsingError("Key without params");
		if (keyValues.second.size() > tmp.second.first) // check if params number exceed max params number
			throw ParsingError("Key with too many params (max params for this key = " + to_string(tmp.second.first) + ")");
		if (!tmp.second.second.empty()) // check if params are in the authorized params list if there is one
		{
			for (std::vector<std::string>::iterator it = keyValues.second.begin(); it != keyValues.second.end(); it++)
				if (!tmp.second.second.count(*it))
					throw ParsingError("Unrecognized param : " + *it);
		}
		if (tmp.first) // call check FUNC if there is one
			tmp.first(keyValues.second);
		if (!location.uniqKey.insert(keyValues).second)
			throw ParsingError("Key already present");
	}
	else if (_keyParsingMap.count(std::make_pair(keyValues.first, NON_UNIQ_KEY)))
	{
		const std::pair<void(*)(std::vector<std::string> &), std::pair<int, std::set<std::string>>>	tmp = _keyParsingMap.find(std::make_pair(keyValues.first, NON_UNIQ_KEY))->second;
		if (keyValues.second.empty())
			throw ParsingError("Key without SubKey");
		std::vector<std::string> params(keyValues.second.begin() + 1, keyValues.second.end());
		if (params.empty())
			throw ParsingError("SubKey without params");
		if (params.size() > tmp.second.first) // check if params number exceed max params number
			throw ParsingError("SubKey with too many params (max params for this SubKey = " + to_string(tmp.second.first) + ")");
		if (!tmp.second.second.empty() && !tmp.second.second.count(keyValues.second[0]))
			throw ParsingError("Unrecognized SubKey : " + keyValues.second[0]);
		if (tmp.first) // call check FUNC if there is one
			tmp.first(keyValues.second);
		if (!location.nonUniqKey[keyValues.first].insert(std::make_pair(keyValues.second[0], params)).second)
			throw ParsingError("SubKey already present");
	}
	else
		throw ParsingError("Unrecognized Key : " + keyValues.first);
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
	std::string	res;
	
	while (lineRange.first != lineRange.second && (_whitespacesSet + _lineBreakSet + _commentSet + _scopeSet).find(*lineRange.first) == std::string::npos)
	{
		if (*lineRange.first == '\\')
			lineRange.first++;
		res.push_back(*lineRange.first++);
	}
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
