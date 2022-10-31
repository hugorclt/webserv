/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/12 21:15:44 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "utils.hpp"

#include <cstdlib>
#include <iostream>
#include <climits>
#include <fstream>

#define MAX_PORT	65535
#define DEFAULT_LISTEN_INTERFACE "0.0.0.0"
#define DEFAULT_LISTEN_PORT "8080"
#define C_RED "\033[1;31m"
#define C_RESET "\033[0m"
#define UNLLIMITED_PARAMS 0
#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)

// patch this shit later
ConfigParser::Conf::data_type	ConfigParser::Conf::_data = ConfigParser::Conf::data_type ();

void	ConfigParser::Conf::init_data(void)
{
	if (!_data.empty())
		return ;
	std::string allow_methodsVP[] = {"GET", "POST", "DELETE"};
	std::string auto_indexVP[] = {"on", "off"};
	std::string error_pageVP[] = {"404", "403", "405"};
	std::string returnVP[] = {"200", "404", "403", "405"};

	std::pair<std::string, raw>	data[] =
	{
		std::make_pair("body_size",     raw(KT_UNIQ, &_checkBodySize, 1)),
		std::make_pair("allow_methods", raw(KT_UNIQ, NULL, 3, allow_methodsVP, SIZEOF(allow_methodsVP))),
		std::make_pair("root",          raw(KT_UNIQ, NULL, 1)),
		std::make_pair("index",         raw(KT_UNIQ, NULL, UNLLIMITED_PARAMS)),
		std::make_pair("auto_index",    raw(KT_UNIQ, NULL, 1, auto_indexVP, SIZEOF(auto_indexVP))),

		std::make_pair("cgi",           raw(KT_NON_UNIQ, &_checkCgi, 1)),
		std::make_pair("error_page",    raw(KT_NON_UNIQ, NULL, 1, error_pageVP, SIZEOF(error_pageVP))),
		std::make_pair("return",        raw(KT_NON_UNIQ, NULL, 1, returnVP, SIZEOF(returnVP))),

		std::make_pair("listen",        raw(KT_SERVER, &formatListen, 2)),
		std::make_pair("server_name",   raw(KT_SERVER, NULL, UNLLIMITED_PARAMS)),
	};
	_data.insert(data, data + SIZEOF(data));
}

ConfigParser::Location	ConfigParser::Conf::_defaultValues = ConfigParser::Location ();

void	ConfigParser::Conf::init_defaultValues(void)
{
	if (!_defaultValues.nonUniqKey.empty() && !_defaultValues.uniqKey.empty())
		return ;
	// nonUniqKey
	std::pair< std::string, std::vector<std::string> >	returnTab[] =
	{
		std::make_pair("200", std::vector<std::string> (1, "OK")),
		std::make_pair("403", std::vector<std::string> (1, "Forbidden")),
		std::make_pair("404", std::vector<std::string> (1, "Not Found")),
		std::make_pair("405", std::vector<std::string> (1, "Method Not Allowed")),
	};

	std::pair< std::string, std::map< std::string, std::vector<std::string> > >	nonUniqKey[] =
	{
		std::make_pair("return", std::map< std::string, std::vector<std::string> > (returnTab, returnTab + SIZEOF(returnTab))),
	};
	_defaultValues.nonUniqKey.insert(nonUniqKey, nonUniqKey + SIZEOF(nonUniqKey));

	// uniqKey
	std::string	allow_methods[] = {"GET", "POST", "DELETE"};

	std::pair< std::string, std::vector<std::string> >	uniqKey[] =
	{
		std::make_pair("auto_index",    std::vector<std::string> (1, "off")),
		std::make_pair("allow_methods", std::vector<std::string> (allow_methods, allow_methods + SIZEOF(allow_methods))),
	};
	_defaultValues.uniqKey.insert(uniqKey, uniqKey + SIZEOF(uniqKey));
}

const std::string ConfigParser::Conf::_whitespacesSet = "\t ";
const std::string ConfigParser::Conf::_lineBreakSet = ";";
const std::string ConfigParser::Conf::_commentSet = "#";
const std::string ConfigParser::Conf::_scopeSet = "{}";

/* -------------------------------------------------------------------------- */
/*                                CheckFunction                               */
/* -------------------------------------------------------------------------- */

static void	checkPort(std::string str) // tmp
{
	int	port;

	if (!isDigits(str))
		throw ConfigParser::ParsingError("listen, port need to be digits", str);
	port = atoi(str.c_str());
	if (port < 0 || port > MAX_PORT)
		throw ConfigParser::ParsingError("listen, port out of range", str);
}

static void checkIp(std::vector<std::string> ip) // tmp
{
	if (ip.size() != 4)
		throw ConfigParser::ParsingError("listen, ip need to be 4 numbers separated by '.'");
	for (std::vector<std::string>::iterator it = ip.begin(); it < ip.end(); it++)
	{
		if (!isDigits(*it))
			throw ConfigParser::ParsingError("listen, ip need to be 4 numbers separated by '.'", *it);
		if (atoi(it->c_str()) < 0 || atoi(it->c_str()) > 255)
			throw ConfigParser::ParsingError("listen, ip numbers need to be between 0 and 255", *it);
	}
}

void	ConfigParser::formatListen(keyValues_type &keyValues)
{
		if (isDigits(keyValues.second[0]))
		{
			checkPort(keyValues.second[0]);
			if (keyValues.second.size() == 2)
			{
				checkIp(split(keyValues.second[1], "."));
				std::swap(keyValues.second[0], keyValues.second[1]);
			}
			else
				keyValues.second.insert(keyValues.second.begin(), DEFAULT_LISTEN_INTERFACE);
		}
		else
		{
			checkIp(split(keyValues.second[0], "."));
			if (keyValues.second.size() == 2)
				checkPort(keyValues.second[1]);
			else
				keyValues.second.push_back(DEFAULT_LISTEN_PORT);
		}
}

void	ConfigParser::_checkCgi(keyValues_type &keyValues)
{
	if (keyValues.first.rfind('.') != 0)
		throw ParsingError("cgi first param should start by a point (the only one)", keyValues.first);
	if (keyValues.first.size() < 2)
		throw ParsingError("cgi first param should have at least one char after the point", keyValues.first);
}

void	ConfigParser::_checkBodySize(keyValues_type &keyValues)
{
	if (!isDigits(keyValues.second[0]))
		throw ParsingError("body_size should be a positive integer", keyValues.second[0]);
	if (atoi(keyValues.second[0].c_str()) <= 0)
		throw ParsingError("body_size overflow, max value : " + to_string(INT_MAX), keyValues.second[0]);
}

/* -------------------------------------------------------------------------- */
/*                                 Constructor                                */
/* -------------------------------------------------------------------------- */

template<class InputIt>
static void	printRange(InputIt first, InputIt last)
{
	if (first == last)
		std::cout << "(NONE)";
	while (first != last)
		std::cout << " " << *first++;
}

template<class T>
static void printMap(const std::map< std::string, T > &map, const std::string &indent)
{
	if (map.empty())
		std::cout << indent << "(NONE)" << std::endl;
	for (typename std::map< std::string, T >::const_iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << indent << "[\"" << it->first << "\"] :";
		printRange(it->second.begin(), it->second.end());
		std::cout << std::endl;
	}
}

void	ConfigParser::_printConfigParser(const data_type &data)
{
	std::cout << "PrintConfigParser :" << std::endl << std::endl;
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
		indent += "\t";
		std::cout << indent << "server_name : ";
		printRange(itServ->server_name.begin(), itServ->server_name.end());
		std::cout << std::endl << std::endl;
		std::cout << indent << "listen" << std::endl
				  << indent << '{' << std::endl;
		printMap(itServ->listen, indent + "\t");
		std::cout << indent << '}' << std::endl
				  << std::endl;
		for (Server::location_type::const_iterator itLoc = itServ->location.begin(); itLoc != itServ->location.end(); itLoc++)
		{
			std::string indent("\t");
			std::cout << indent << "location [\"" << itLoc->first << "\"] :" << std::endl;
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
			for (Location::nonUniqKey_type::const_iterator it = itLoc->second.nonUniqKey.begin(); it != itLoc->second.nonUniqKey.end(); it++)
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
		indent = "";
		std::cout << indent << '}' << std::endl << std::endl << std::endl << std::endl;
	}
}

ConfigParser::ConfigParser(char *filename) {
	Conf::init_data();
	Conf::init_defaultValues();

	std::ifstream				input(filename);
	std::vector<std::string>	fullFile;

	for (std::string line; std::getline(input, line); ) {
		fullFile.push_back(line);
	}
	
	if (fullFile.empty())
	{
		_printConfigParser(_data);
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
				try { _data.push_back(_createNewServer(lineRange, fileRange)); }
				catch (ParsingError &e) { throw ParsingError("Server " + to_string(_data.size()) + " : " + e.what(), e.word()); }
			}
			else if (fileRange.first != fileRange.second)
				throw ParsingError("wrong Token Global Scope");
			_goToNextWordInFile(lineRange, fileRange);
		}
	}
	catch (ParsingError &e)
	{
		std::string line;
		if (fileRange.first == fileRange.second)
			line = *(fileRange.first - 1);
		else
			line = _color(std::string(fileRange.first->begin(), lineRange.first), e.word()) + std::string(lineRange.first, lineRange.second);
		throw ParsingError(std::string(e.what()) + " :\n"
		+ "line " + to_string(fullFile.size() - (fileRange.second - fileRange.first) + 1) + " : "
		+ line);
	}
	_printConfigParser(_data);
}

//tmp test
std::string	ConfigParser::_color(std::string line, std::string word)
{
	if (line.rfind(word) != std::string::npos)
	{
		line.insert(line.rfind(word), C_RED);
		line.insert(line.rfind(word) + word.size(), C_RESET);
	}
	return (line);
}

/* -------------------------------------------------------------------------- */
/*                               ConfigParserFunction                              */
/* -------------------------------------------------------------------------- */

bool	ConfigParser::_isServer(keyValues_type keyValues, lineRange_type &lineRange, fileRange_type &fileRange)
{
	if (keyValues.first != "server")
		return (false);
	if (!keyValues.second.empty())
		throw ParsingError("server doesn't take param");
	_goToNextWordInFile(lineRange, fileRange);
	return (*lineRange.first == '{');
}

bool	ConfigParser::_isLocation(keyValues_type keyValues, lineRange_type &lineRange, fileRange_type &fileRange)
{
	lineRange_type	lineRangeCpy = lineRange;
	fileRange_type	fileRangeCpy = fileRange;
	_goToNextWordInFile(lineRangeCpy, fileRangeCpy);
	if (keyValues.first != "location")
		return (false);
	if (keyValues.second.size() != 1)
		throw ParsingError("location take one param");
	if (keyValues.second[0][0] != '/')
		throw ParsingError("location need to start by a '/'", keyValues.second[0]);
	if (*lineRangeCpy.first != '{')
		throw ParsingError("location : can't find '{'");
	return (true);
}

ConfigParser::keyValues_type	ConfigParser::_getKeyValues(lineRange_type &lineRange)
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

void			ConfigParser::Conf::checkKeyValues(keyValues_type &keyValues, const raw &keyConf)
{
	std::set<std::string>	sParams(keyValues.second.begin(), keyValues.second.end());
	if (keyValues.second.empty())
		throw ParsingError("Key without enough params", keyValues.first);
	if (!keyConf.validParams.empty()) // If a set of valid param is provided
	{
		if (keyConf.kt == KT_UNIQ) // check than all params are present in this set
		{
			for (std::set<std::string>::iterator it = sParams.begin(); it != sParams.end(); it++)
				if (!keyConf.validParams.count(*it))
					throw ParsingError("Unknown param", *it);
		}
		else if (keyConf.kt == KT_NON_UNIQ && !keyConf.validParams.count(keyValues.first)) // check than SubKey is present in this set
			throw ParsingError("Unknown param", keyValues.first);
	}
	if (sParams.size() != keyValues.second.size())
		throw ParsingError("duplicated params");
	if (keyConf.maxParams && sParams.size() > keyConf.maxParams)
		throw ParsingError("Key with too many params (max : " + to_string(keyConf.maxParams) + ")", keyValues.first);
	if (keyConf.func) // if a check function is provided, call it
		keyConf.func(keyValues);
}

void			ConfigParser::_insertKeyValuesInLocation(Location &location, keyValues_type &keyValues) // still need to be cleaned
{
	Conf::raw	keyConf = Conf::_data[keyValues.first];
	Location::uniqKey_type	&insertionPoint = (keyConf.kt == Conf::KT_UNIQ) ? location.uniqKey : location.nonUniqKey[keyValues.first];

	if (keyConf.kt == Conf::KT_NON_UNIQ && !keyValues.second.empty()) // set SubKey as keyValues.first and SubKey params as keyValues.second
	{
		keyValues.first = keyValues.second[0];
		keyValues.second.erase(keyValues.second.begin());
	}
	Conf::checkKeyValues(keyValues, keyConf);
	if (!insertionPoint.insert(std::make_pair(keyValues.first, keyValues.second)).second)
		throw ParsingError("Key already present", keyValues.first);
}

ConfigParser::Location	ConfigParser::_createNewLocation(lineRange_type &lineRange, fileRange_type &fileRange)
{
	Location	res;

	_goToNextWordInFile(lineRange, fileRange);
	keyValues_type	keyValues = _getKeyValues(lineRange);
	while (fileRange.first != fileRange.second && !keyValues.first.empty())
	{
		if (Conf::getKeyType(keyValues.first) == Conf::KT_UNIQ || Conf::getKeyType(keyValues.first) == Conf::KT_NON_UNIQ)
			_insertKeyValuesInLocation(res, keyValues);
		else if (Conf::getKeyType(keyValues.first) == Conf::KT_SERVER)
			throw ParsingError("Key '" + keyValues.first + "' is a Server Key");
		else
			throw ParsingError("Unknown Key '" + keyValues.first + "'");
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

void	ConfigParser::_insertKeyValuesInServer(Server &res, keyValues_type &keyValues)
{
	const Conf::raw	keyConf = Conf::_data.find(keyValues.first)->second;
	Conf::checkKeyValues(keyValues, keyConf);
	if (keyValues.first == "listen" && !res.listen[keyValues.second[0]].insert(keyValues.second[1]).second)
		throw ParsingError("already listening on " + keyValues.second[0] + ":" + keyValues.second[1]);
	else if (keyValues.first == "server_name")
	{
		size_t	oldSize = res.server_name.size();
		res.server_name.insert(keyValues.second.begin(), keyValues.second.end());
		if (res.server_name.size() != oldSize + keyValues.second.size())
			throw ParsingError("server_name already present");
	}
}

ConfigParser::Server	ConfigParser::_createNewServer(lineRange_type &lineRange, fileRange_type &fileRange)
{
	Server	res;
	Location	serverLocationConf;
	
	_goToNextWordInFile(lineRange, fileRange);
	keyValues_type	keyValues = _getKeyValues(lineRange);
	while (fileRange.first != fileRange.second && !keyValues.first.empty())
	{
		if (_isLocation(keyValues, lineRange, fileRange))
		{
			if (res.location.count(keyValues.second[0]))
				throw ParsingError("Location duplication : \"" + keyValues.second[0] + "\"");
			_goToNextWordInFile(lineRange, fileRange); //skip the location
			lineRange.first++; //skip the location
			try { res.location.insert(std::make_pair(keyValues.second[0], _createNewLocation(lineRange, fileRange))); }
			catch (ParsingError &e) { throw ParsingError("location \"" + keyValues.second[0] + "\" : " + e.what(), e.word()); }
		}
		else
		{
			if (Conf::getKeyType(keyValues.first) == Conf::KT_UNIQ || Conf::getKeyType(keyValues.first) == Conf::KT_NON_UNIQ)
				_insertKeyValuesInLocation(serverLocationConf, keyValues);
			else if (Conf::getKeyType(keyValues.first) == Conf::KT_SERVER)
				_insertKeyValuesInServer(res, keyValues);
			else
				throw ParsingError("Unknown Key '" + keyValues.first + "'");
		}
		_goToNextWordInFile(lineRange, fileRange);
		keyValues = _getKeyValues(lineRange);
	}
	if (*lineRange.first == '{')
		throw ParsingError("Wrong { token inServer (not related to a Location)");
	if (fileRange.first == fileRange.second)
		throw ParsingError("UnclosedServer");
	lineRange.first++;
	res.location["/"].insert(serverLocationConf);
	res.location["/"].insert(Conf::_defaultValues);
	return (res);
}

void	ConfigParser::_skipCharset(lineRange_type &lineRange, const std::string &charset)
{ 
	while (lineRange.first != lineRange.second && charset.find(*lineRange.first) != std::string::npos)
		lineRange.first++;
	if (lineRange.first != lineRange.second && Conf::_commentSet.find(*lineRange.first) != std::string::npos)
		lineRange.first = lineRange.second;
}

void	ConfigParser::_goToNextWordInFile(lineRange_type &lineRange, fileRange_type &fileRange)
{
	if (fileRange.first == fileRange.second)
		return ;
	_skipCharset(lineRange, Conf::_whitespacesSet + Conf::_lineBreakSet);
	while (fileRange.first != fileRange.second && lineRange.first == lineRange.second) {
		fileRange.first++;
		if (fileRange.first == fileRange.second)
			break ;
		lineRange.first = fileRange.first->begin();
		lineRange.second = fileRange.first->end();
		_skipCharset(lineRange, Conf::_whitespacesSet + Conf::_lineBreakSet);
	}
}

std::string	ConfigParser::_getWord(lineRange_type &lineRange)
{
	std::string	res;
	
	while (lineRange.first != lineRange.second && (Conf::_whitespacesSet + Conf::_lineBreakSet +
		   Conf::_commentSet + Conf::_scopeSet).find(*lineRange.first) == std::string::npos)
	{
		if (*lineRange.first == '\\')
			lineRange.first++;
		if (lineRange.first == lineRange.second)
			throw ParsingError("'\\' need to be used in combination with either an other char or itself");
		res.push_back(*lineRange.first++);
	}
	return (res);
}

std::string	ConfigParser::_getWordSkipSpace(lineRange_type &lineRange)
{
	_skipCharset(lineRange, Conf::_whitespacesSet);
	std::string word = _getWord(lineRange);
	_skipCharset(lineRange, Conf::_whitespacesSet);
	return (word);
}

/* -------------------------------------------------------------------------- */
/*                                  accessor                                  */
/* -------------------------------------------------------------------------- */

ConfigParser::data_type	ConfigParser::getData(void) {
	return (_data);
}
