/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                         :+:      :+:    :+:   */
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

class ConfigParser {
	private:
		typedef std::string::iterator								lineIt_type;
		typedef std::vector<std::string>::iterator					fileIt_type;
		typedef std::pair<lineIt_type, lineIt_type>					lineRange_type;
		typedef std::pair<fileIt_type, fileIt_type>					fileRange_type;
		typedef std::pair<std::string, std::vector<std::string>>	keyValues_type;

		
	public:

		struct Location
		{
			typedef std::map<std::string, std::map<std::string, std::set<std::string>>>	nonUniqKey_type; // Usage Ex : Conf._data[0].locations.nonUniqKey["error_pages"]["404"] -> ./404.html
			typedef std::map<std::string, std::set<std::string>>						uniqKey_type; // Usage Ex : Conf_data[0].location.uniqKey["root"] -> ./var/srv
			
			nonUniqKey_type	nonUniqKey;
			uniqKey_type	uniqKey;

			static void			_insertUniqKey(const uniqKey_type &a, uniqKey_type &b)
			{
				for (uniqKey_type::const_iterator it = a.begin(); it != a.end(); it++)
					b.insert(*it);
			}

			void			insert(const Location &otherInst)
			{
				_insertUniqKey(otherInst.uniqKey, uniqKey);
				for (nonUniqKey_type::const_iterator it = otherInst.nonUniqKey.begin(); it != otherInst.nonUniqKey.end(); it++)
					_insertUniqKey(it->second, nonUniqKey[it->first]);
			}
		};

		struct Server
		{
			typedef std::map<std::string, Location>			location_type;
			typedef std::map<std::string, std::set<std::string>>	listen_type;

			std::map<std::string, std::set<std::string>>	listen; // map d'{Ip, set<port>}
			std::string										server_name; // set de server_name
			location_type									location;
		};

	private:
		struct Conf
		{
			enum KeyType {KT_NONE, KT_SERVER, KT_UNIQ, KT_NON_UNIQ};

			struct raw
			{
				KeyType						kt;
				void						(*func)(keyValues_type &);
				int							maxParams;
				std::set<std::string>		validParams;
			};

			typedef std::map<std::string, raw>					data_type;

			const static data_type		_data;
			const static Location		_defaultValues;
			const static std::string	_whitespacesSet;
			const static std::string	_lineBreakSet;
			const static std::string	_commentSet;
			const static std::string	_scopeSet;

			static void		checkKeyValues(keyValues_type &keyValues, const raw &keyConf);
			static KeyType	getKeyType(const std::string &key)
			{
				if (_data.find(key) != _data.end())
					return (_data.find(key)->second.kt);
				else
					return (KT_NONE);
			}
		};

	public:

		typedef std::vector<Server> 						data_type;
		typedef std::map<std::string, std::vector<std::string>>	map_type;
		
	private:
		data_type	_data;
	
		//parseFunction
		void			_skipCharset(lineRange_type &lineRange, const std::string &charset);
		std::string		_getWordSkipSpace(lineRange_type &strIt);
		std::string		_getWord(lineRange_type &strIt);
		void			_goToNextWordInFile(lineRange_type &strIt, fileRange_type &fileIt);
		bool			_isServer(keyValues_type keyValues, lineRange_type &lineRange, fileRange_type &fileRange);
		bool			_isLocation(keyValues_type pair, lineRange_type &strIt, fileRange_type &fileIt);
		Server			_createNewServer(lineRange_type &strIt, fileRange_type &fileIt);
		Location		_createNewLocation(lineRange_type &strIt, fileRange_type &fileIt);
		keyValues_type	_getKeyValues(lineRange_type &strIt);
		void			_insertKeyValuesInLocation(Location &location, keyValues_type &keyValues);
		void			_insertKeyValuesInServer(Server &res, keyValues_type &keyValues);

		//Check Functions
		static void	_checkBodySize(keyValues_type &keyValues);
		static void	_checkCgi(keyValues_type &keyValues);
		static void	formatListen(keyValues_type &keyValues);
		/*
		static bool	_checkListen(std::vector<std::string> &vec);
		bool		_checkIpHost(void);
		*/

		//test

		static void	_printConfigParser(const data_type &data);
		
	public:
		ConfigParser(char *params);

		//accessor
		data_type	getData(void);
		
		class ParsingError: public std::exception {
			private:
				std::string	_error;
			public:
				ParsingError(std::string error) : _error(error) {};
				virtual const char *what() const throw() {
					return (_error.c_str());
				}
		};
};
