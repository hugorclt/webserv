/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                         :+:      :+:    :+:   */
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



class Parsing {
	private:
		typedef std::string::iterator								lineIt_type;
		typedef std::vector<std::string>::iterator					fileIt_type;
		typedef std::pair<lineIt_type, lineIt_type>					lineRange_type;
		typedef std::pair<fileIt_type, fileIt_type>					fileRange_type;
		typedef std::pair<std::string, std::vector<std::string>>	keyValues_type;

		struct _Location
		{
			typedef std::map<std::string, std::map<std::string, std::set<std::string>>>	nonUniqKey_type; // Usage Ex : Conf._data[0].locations.nonUniqKey["error_pages"]["404"] -> ./404.html
			typedef std::map<std::string, std::set<std::string>>						uniqKey_type; // Usage Ex : Conf_data[0].location.uniqKey["root"] -> ./var/srv
			
			nonUniqKey_type	nonUniqKey;
			uniqKey_type	uniqKey;
		};

		struct _Server
		{
			typedef std::map<std::string, std::vector<std::string>>	confType;
			typedef std::map<std::string, _Location>			locationType;

			std::map<std::string, std::set<std::string>>	listen; // map d'{Ip, set<port>}
			std::set<std::string>							server_name; // set de server_name
			locationType	location;
		};

		struct _Config
		{
			enum KeyType {KT_NONE, KT_SERVER, KT_UNIQ, KT_NON_UNIQ};

			typedef std::pair<void(*)(keyValues_type &), std::pair<int, std::set<std::string>>>	dataValue_type;
			typedef std::map<std::pair<std::string, KeyType>, dataValue_type>					data_type;

			const static data_type		_data;
			const static std::string	_whitespacesSet;
			const static std::string	_lineBreakSet;
			const static std::string	_commentSet;
			const static std::string	_scopeSet;

			static KeyType	getKeyType(const std::string &key)
			{
				const KeyType	keyTypeSet[] = {KT_SERVER, KT_UNIQ, KT_NON_UNIQ};
				for (size_t i = 0; i < 3; i++)
					if (_data.count({key, keyTypeSet[i]}))
						return (keyTypeSet[i]);
				return (KT_NONE);
			}
		};

	public:
		typedef std::vector<_Server> 						data_type;
		typedef std::map<std::string, std::vector<std::string>>	map_type;
		
	private:
		data_type					_data;
	
		//parseFunction
		void			_skipCharset(lineRange_type &lineRange, const std::string &charset);
		std::string		_getWordSkipSpace(lineRange_type &strIt);
		std::string		_getWord(lineRange_type &strIt);
		void			_goToNextWordInFile(lineRange_type &strIt, fileRange_type &fileIt);
		bool			_isServer(keyValues_type keyValues, lineRange_type &lineRange, fileRange_type &fileRange);
		bool			_isLocation(keyValues_type pair, lineRange_type &strIt, fileRange_type &fileIt);
		_Server			_createNewServer(lineRange_type &strIt, fileRange_type &fileIt);
		_Location		_createNewLocation(lineRange_type &strIt, fileRange_type &fileIt);
		keyValues_type	_getKeyValues(lineRange_type &strIt);
		void			_insertKeyValuesInLocation(_Location &location, keyValues_type &keyValues);

		//Check Functions
		static void	_checkBodySize(keyValues_type &keyValues);
		static void	_checkCgi(keyValues_type &keyValues);
		/*
		static bool	_checkListen(std::vector<std::string> &vec);
		bool		_checkIpHost(void);
		*/

		//test

		static void	_printParsing(const data_type &data);
		
	public:
		Parsing(char *params);

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
