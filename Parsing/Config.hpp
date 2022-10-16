/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:05 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/12 21:13:12 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

#define MAX_PORT	65535

struct LocationConfig
{
	typedef std::map<std::string, std::map<std::string, std::vector<std::string>>>	nonUniqKey_type; // Usage Ex : Conf._data[0].locations.nonUniqKey["error_pages"]["404"] -> ./404.html
	typedef std::map<std::string, std::vector<std::string>>							uniqKey_type; // Usage Ex : Conf_data[0].location.uniqKey["root"] -> ./var/srv
	
	nonUniqKey_type	nonUniqKey;
	uniqKey_type	uniqKey;
};

struct ServerConfig
{
	typedef std::map<std::string, std::vector<std::string>>	confType;
	typedef std::map<std::string, LocationConfig>			locationType;

	std::map<std::string, std::set<std::string>>	listen; //map d'ip puis set de port pour cette ip
	std::set<std::string>							server_name; //set de server_name
	locationType	location;
};


class Config {
	private:
		typedef std::string::iterator								lineIt_type;
		typedef std::vector<std::string>::iterator					fileIt_type;
		typedef std::pair<lineIt_type, lineIt_type>					lineRange_type;
		typedef std::pair<fileIt_type, fileIt_type>					fileRange_type;
		typedef std::pair<std::string, std::vector<std::string>>	keyValues_type;

	public:
		typedef std::vector<ServerConfig> 						data_type;
		typedef std::map<std::string, std::vector<std::string>>	map_type;
		
	private:
		int							_nbServer;
		data_type					_data;
		const static std::map<std::string, bool(*)(std::vector<std::string> &)>	_serverKey;

		enum _KeyType {UNIQ_KEY, NON_UNIQ_KEY};
		const static std::map<std::pair<std::string, _KeyType>, std::pair<void(*)(std::vector<std::string> &), std::pair<int, std::set<std::string>>>>	_keyParsingMap;

		const static std::string	_whitespacesSet;
		const static std::string	_lineBreakSet;
		const static std::string	_commentSet;
		const static std::string	_scopeSet;
	
		//parseFunction
		std::string													_getWordSkipSpace(lineRange_type &strIt);
		std::string													_getWord(lineRange_type &strIt);
		void														_goToNextWordInFile(lineRange_type &strIt, fileRange_type &fileIt);
		void														_skipCharset(lineRange_type &lineRange, const std::string &charset);
		void														_skipSpace(lineRange_type &strIt);
		std::pair<std::string, bool(*)(std::vector<std::string> &)>	_getOpt(std::string key);
		bool														_isServer(keyValues_type pair, lineRange_type &strIt, fileRange_type &fileIt);
		bool														_isLocation(keyValues_type pair, lineRange_type &strIt, fileRange_type &fileIt);
		ServerConfig												_createNewServerConfig(lineRange_type &strIt, fileRange_type &fileIt);
		LocationConfig												_createNewLocation(lineRange_type &strIt, fileRange_type &fileIt);

		keyValues_type												_getKeyValues(lineRange_type &strIt);
		void														_insertKeyValuesInLocation(LocationConfig &location, keyValues_type &keyValues);

		//Check Functions
		static bool	_isServerKey(const std::string &key);
		static bool	_isUniqKey(const std::string &key);
		static bool	_isNonUniqKey(const std::string &key);
		static bool	_isValidKey(const std::string &key);

		//static bool	_checkAutoIndex(std::vector<std::string> &vec);
		static void	_checkBodySize(std::vector<std::string> &vec);
		static void	_checkCgi(std::vector<std::string> &vec);
		/*
		static bool	_checkPath(std::vector<std::string> &vec);
		static bool	_checkListen(std::vector<std::string> &vec);
		bool		_checkAllValue(map_type	&serverConfig);
		bool		_checkIpHost(void);
		bool		_checkKeyConfServer(ServerConfig::confType &confServ);
		bool		_checkKeyLocation(ServerConfig::locationType &confLocation);
		*/

		//test

		static void	_printConfig(const data_type &data);



		
	public:
		Config(char *params);

		//accessor
		data_type	getData(void);
		
		class ParsingError : public std::exception {
			private:
				std::string	_error;
			public:
				ParsingError(std::string error) : _error(error) {};
				virtual const char *what() const throw() {
					return (_error.c_str());
				}
		};
};

