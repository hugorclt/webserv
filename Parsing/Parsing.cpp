/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/12 21:15:44 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include <climits>
#define DEFAULT_LISTEN_INTEFACE "0.0.0.0"
#define DEFAULT_LISTEN_PORT "80"

const Parsing::_Config::data_type	Parsing::_Config::_data
{
	{{"body_size"    , KT_UNIQ    }, {&_checkBodySize, {1, {                       }}}},
	{{"allow_methods", KT_UNIQ    }, {NULL           , {3, {"GET", "POST", "DELETE"}}}},
	{{"root"         , KT_UNIQ    }, {NULL           , {1, {                       }}}},
	{{"index"        , KT_UNIQ    }, {NULL           , {1, {                       }}}},
	{{"auto_index"   , KT_UNIQ    }, {NULL           , {1, {"on", "off"            }}}},

	{{"cgi"          , KT_NON_UNIQ}, {&_checkCgi     , {1, {                       }}}},
	{{"error_page"   , KT_NON_UNIQ}, {NULL           , {1, {"404", "403", "442"    }}}},

	{{"listen"       , KT_SERVER  }, {NULL           , {0, {                       }}}},
	{{"server_name"  , KT_SERVER  }, {NULL           , {0, {                       }}}}
};

const std::string Parsing::_Config::_whitespacesSet = "\t ";
const std::string Parsing::_Config::_lineBreakSet = ";";
const std::string Parsing::_Config::_commentSet = "#";
const std::string Parsing::_Config::_scopeSet = "{}";

/* -------------------------------------------------------------------------- */
/*                                CheckFunction                               */
/* -------------------------------------------------------------------------- */

/* DUMB SHIT LOL
static void	checkPort(std::string str) // tmp
{
	int	port;

	if (!isDigits(str))
		throw ParsingError("_listen");
	port = atoi(str.c_str());
	if (port < 0 || port > MAX_PORT)
		throw ParsingError("_listen");
}

static void checkIp(std::string str) // tmp
{
	std::vector<std::string>	ip;

	ip = split(keyValues.second[1], ".");
	if (ip.size() != 4)
		throw ParsingError("_listen");
	for (std::vector<std::string>::iterator it = ip.begin(); it < ip.end(); it++)
	{
		if (atoi(it->c_str()) < 0 || atoi(it->c_str()) > 255)
			throw ParsingError("_listen");
	}
}

void	Parsing::_listen(keyValues_type &keyValues)
{
	if (keyValues.second.size() == 2)
	{
		if (isDigits(keyValues.second[0]))
		{
			checkPort(keyValues.second[0]);
			checkIp(keyValues.second[1]);
			std::swap(keyValues.second[0], keyValues.second[1]);
		}
		else
		{
			checkPort(keyValues.second[1]);
			checkIp(keyValues.second[0]);
		}
	}
	else
	{
		if (isDigits(KeyValues.second[0]))
		{
			checkPort(keyValues.second[0]));
			keyValues.second.insert(keyValue.second.begin(), DEFAULT_LISTEN_INTERFACE);
		}
		else
		{
			checkIp(keyValues.second[0]));
			keyValues.second.push_back(DEFAULT_LISTEN_PORT);
		}
	}
}
*/

void	Parsing::_checkCgi(keyValues_type &keyValues)
{
	if (keyValues.first.rfind('.') != 0)
		throw ParsingError("cgi first param should start by a point (the only one)");
	if (keyValues.first.size() < 2)
		throw ParsingError("cgi first param should have at least one char after the point");
}

void	Parsing::_checkBodySize(keyValues_type &keyValues)
{
	if (!isDigits(keyValues.second[0]))
		throw ParsingError("body_size should be a positive integer");
	if (atoi(keyValues.second[0].c_str()) <= 0)
		throw ParsingError("body_size overflow, max value : " + to_string(INT_MAX));
}

/* -------------------------------------------------------------------------- */
/*                                 Constructor                                */
/* -------------------------------------------------------------------------- */

static void	printSet(const std::set<std::string> &set)
{
	if (set.empty())
		std::cout << "(NONE)";
	for (std::set<std::string>::const_iterator it = set.begin(); it != set.end(); it++)
		std::cout << " " << *it;
}

static void printMap(const std::map<std::string, std::set<std::string>> &map, const std::string &indent)
{
	if (map.empty())
		std::cout << indent << "(NONE)" << std::endl;
	for (std::map<std::string, std::set<std::string>>::const_iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << indent << "[\"" << it->first << "\"] :";
		printSet(it->second);
		std::cout << std::endl;
	}
}

void	Parsing::_printParsing(const data_type &data)
{
	std::cout << "PrintParsing :" << std::endl << std::endl;
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
		for (_Server::locationType::const_iterator itLoc = itServ->location.begin(); itLoc != itServ->location.end(); itLoc++)
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
			for (_Location::nonUniqKey_type::const_iterator it = itLoc->second.nonUniqKey.begin(); it != itLoc->second.nonUniqKey.end(); it++)
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

Parsing::Parsing(char *filename) {
	std::ifstream				input(filename);
	std::vector<std::string>	fullFile;

	for (std::string line; std::getline(input, line); ) {
		fullFile.push_back(line);
	}
	
	if (fullFile.empty())
	{
		_printParsing(_data);
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
	_printParsing(_data);
}

/* -------------------------------------------------------------------------- */
/*                               ParsingFunction                              */
/* -------------------------------------------------------------------------- */

bool	Parsing::_isServer(keyValues_type keyValues, lineRange_type &lineRange, fileRange_type &fileRange)
{
	if (keyValues.first != "server")
		return (false);
	if (!keyValues.second.empty())
		throw ParsingError("server doesn't take param");
	_goToNextWordInFile(lineRange, fileRange);
	return (*lineRange.first == '{');
}

bool	Parsing::_isLocation(keyValues_type keyValues, lineRange_type &lineRange, fileRange_type &fileRange)
{
	lineRange_type	lineRangeCpy = lineRange;
	fileRange_type	fileRangeCpy = fileRange;
	_goToNextWordInFile(lineRangeCpy, fileRangeCpy);
	if (keyValues.first != "location")
		return (false);
	if (keyValues.second.size() != 1)
		throw ParsingError("location take one param");
	if (*lineRangeCpy.first != '{')
		throw ParsingError("location : can't find '{'");
	return (true);
}

Parsing::keyValues_type	Parsing::_getKeyValues(lineRange_type &lineRange)
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

void			Parsing::_insertKeyValuesInLocation(_Location &location, keyValues_type &keyValues) // still need to be cleaned
{
	_Config::KeyType	kt = _Config::getKeyType(keyValues.first);

	if (kt == _Config::KT_NONE)
		throw ParsingError("Unknown Key '" + keyValues.first + "'");
	// Get keyParsingMap data
	const _Config::dataValue_type	tmp = _Config::_data.find({keyValues.first, kt})->second;
	// Get insertionPoint
	_Location::uniqKey_type	&insertionPoint = (kt == _Config::KT_UNIQ) ? location.uniqKey : location.nonUniqKey[keyValues.first];

	if (kt == _Config::KT_NON_UNIQ && !keyValues.second.empty()) // set SubKey as keyValues.first and SubKey params as keyValues.second
	{
		keyValues.first = keyValues.second[0];
		keyValues.second.erase(keyValues.second.begin());
	}
	std::set<std::string>	sParams(keyValues.second.begin(), keyValues.second.end());
	if (keyValues.second.empty())
		throw ParsingError("Key without enough params");
	if (!tmp.second.second.empty()) // If a set of valid param is provided
	{
		if (kt == _Config::KT_UNIQ) // check than all params are present in this set
		{
			for (std::set<std::string>::iterator it = sParams.begin(); it != sParams.end(); it++)
				if (!tmp.second.second.count(*it))
					throw ParsingError("Unknown param '" + *it + "'");
		}
		else if (!tmp.second.second.count(keyValues.first)) // check than SubKey is present in this set
			throw ParsingError("Unknown param '" + keyValues.first + "'");
	}
	if (sParams.size() != keyValues.second.size())
		throw ParsingError("duplicated params");
	if (sParams.size() > tmp.second.first)
		throw ParsingError("Key with too many params (max : " + to_string(tmp.second.first) + ")");
	if (tmp.first) // if a check function is provided, call it
		tmp.first(keyValues);
	if (!insertionPoint.insert({keyValues.first, sParams}).second)
		throw ParsingError("Key already present");
}

Parsing::_Location	Parsing::_createNewLocation(lineRange_type &lineRange, fileRange_type &fileRange)
{
	_Location	res;

	_goToNextWordInFile(lineRange, fileRange);
	keyValues_type	keyValues = _getKeyValues(lineRange);
	while (fileRange.first != fileRange.second && !keyValues.first.empty())
	{
		_insertKeyValuesInLocation(res, keyValues);
		_goToNextWordInFile(lineRange, fileRange);
		keyValues = _getKeyValues(lineRange);
	}
	if (*lineRange.first == '{')
		throw ParsingError("Wrong { token in _Location");
	if (fileRange.first == fileRange.second)
		throw ParsingError("Unclosed _Location");
	lineRange.first++;
	_goToNextWordInFile(lineRange, fileRange);
	return (res);
}

Parsing::_Server	Parsing::_createNewServer(lineRange_type &lineRange, fileRange_type &fileRange)
{
	_Server	res;
	_Location	server_LocationConf;
	
	_goToNextWordInFile(lineRange, fileRange);
	keyValues_type	keyValues = _getKeyValues(lineRange);
	while (fileRange.first != fileRange.second && !keyValues.first.empty())
	{
		if (_isLocation(keyValues, lineRange, fileRange))
		{
			if (res.location.find(keyValues.second[0]) != res.location.end())
				throw ParsingError("_Location duplication : \"" + keyValues.second[0] + "\"");
			_goToNextWordInFile(lineRange, fileRange); //skip the location
			lineRange.first++; //skip the location
			try { res.location.insert(std::make_pair(keyValues.second[0], _createNewLocation(lineRange, fileRange))); }
			catch (ParsingError &e) { throw ParsingError("location \"" + keyValues.second[0] + "\" : " + e.what()); }
		}
		else
		{
			if (_Config::getKeyType(keyValues.first) != _Config::KT_SERVER)
				_insertKeyValuesInLocation(server_LocationConf, keyValues);
			//_insertKeyValuesIServer(res, keyValues);
		}
		_goToNextWordInFile(lineRange, fileRange);
		keyValues = _getKeyValues(lineRange);
	}
	if (*lineRange.first == '{')
		throw ParsingError("Wrong { token inServer (not related to a _Location)");
	if (fileRange.first == fileRange.second)
		throw ParsingError("UnclosedServer");
	lineRange.first++;
	for (_Location::uniqKey_type::iterator it = server_LocationConf.uniqKey.begin(); it != server_LocationConf.uniqKey.end(); it++)
		res.location["/"].uniqKey.insert(*it);
	for (_Location::nonUniqKey_type::iterator it = server_LocationConf.nonUniqKey.begin(); it != server_LocationConf.nonUniqKey.end(); it++)
		for (_Location::uniqKey_type::iterator nit = it->second.begin(); nit != it->second.end(); nit++)
			res.location["/"].nonUniqKey[it->first].insert(*nit);
	return (res);
}

void	Parsing::_skipCharset(lineRange_type &lineRange, const std::string &charset)
{ 
	while (lineRange.first != lineRange.second && charset.find(*lineRange.first) != std::string::npos)
		lineRange.first++;
	if (lineRange.first != lineRange.second && _Config::_commentSet.find(*lineRange.first) != std::string::npos)
		lineRange.first = lineRange.second;
}

void	Parsing::_goToNextWordInFile(lineRange_type &lineRange, fileRange_type &fileRange)
{
	if (fileRange.first == fileRange.second)
		return ;
	_skipCharset(lineRange, _Config::_whitespacesSet + _Config::_lineBreakSet);
	while (fileRange.first != fileRange.second && lineRange.first == lineRange.second) {
		fileRange.first++;
		if (fileRange.first == fileRange.second)
			break ;
		lineRange.first = fileRange.first->begin();
		lineRange.second = fileRange.first->end();
		_skipCharset(lineRange, _Config::_whitespacesSet + _Config::_lineBreakSet);
	}
}

std::string	Parsing::_getWord(lineRange_type &lineRange)
{
	std::string	res;
	
	while (lineRange.first != lineRange.second && (_Config::_whitespacesSet + _Config::_lineBreakSet +
		   _Config::_commentSet + _Config::_scopeSet).find(*lineRange.first) == std::string::npos)
	{
		if (*lineRange.first == '\\')
			lineRange.first++;
		if (lineRange.first == lineRange.second)
			throw ParsingError("'\\' need to be used in combination with either an other char or itself");
		res.push_back(*lineRange.first++);
	}
	return (res);
}

std::string	Parsing::_getWordSkipSpace(lineRange_type &lineRange)
{
	_skipCharset(lineRange, _Config::_whitespacesSet);
	std::string word = _getWord(lineRange);
	_skipCharset(lineRange, _Config::_whitespacesSet);
	return (word);
}

/* -------------------------------------------------------------------------- */
/*                                  accessor                                  */
/* -------------------------------------------------------------------------- */

Parsing::data_type	Parsing::getData(void) {
	return (_data);
}
