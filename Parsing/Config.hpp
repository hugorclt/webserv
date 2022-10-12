/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:05 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/12 11:43:20 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

#define N_OPT		7
#define MAX_PORT	65535

template <class Iterator>
struct RangeIterator
{
	Iterator first;
	Iterator second;

	RangeIterator(Iterator &start, Iterator &end) : first(start), second(end) {};
};

struct ServerConfig
{
	typedef std::map<std::string, std::vector<std::string>>							confType;
	typedef std::map<std::string, std::map<std::string, std::vector<std::string>>>	locationType;

	confType		conf;
	locationType	location;
};


class Config {
	public:
		typedef std::vector<ServerConfig> 	data_type;
		typedef std::map<std::string, std::vector<std::string>>	map_type;
		
	private:
		int							_nbServer;
		data_type					_data;
		const static std::pair<std::string, bool(*)(std::vector<std::string> &)>	_option[N_OPT];
	
		//parseFunction
		std::string													_getWordSkipSpace(RangeIterator<std::string::iterator> &strIt);
		std::string													_getWord(RangeIterator<std::string::iterator> &strIt);
		void														_skipLineEmpty(RangeIterator<std::string::iterator> &strIt, RangeIterator<std::vector<std::string>::iterator> &fileIt);
		void														_skipSpace(RangeIterator<std::string::iterator> &strIt);
		std::pair<std::string, bool(*)(std::vector<std::string> &)>	_getOpt(std::string key);
		bool														_isServer(std::pair<std::string, std::vector<std::string>> pair, RangeIterator<std::string::iterator> &strIt, RangeIterator<std::vector<std::string>::iterator> &fileIt);
		bool														_isLocation(std::pair<std::string, std::vector<std::string>> pair, RangeIterator<std::string::iterator> &strIt, RangeIterator<std::vector<std::string>::iterator> &fileIt);
		ServerConfig												_createNewServerConfig(RangeIterator<std::string::iterator> &strIt, RangeIterator<std::vector<std::string>::iterator> &fileIt);
		ServerConfig::confType										_createNewLocation(RangeIterator<std::string::iterator> &strIt, RangeIterator<std::vector<std::string>::iterator> &fileIt);
		std::pair<std::string, std::vector<std::string>>			_getKeyValuePair(RangeIterator<std::string::iterator> &strIt);

		//Check Functions
		static bool	_isValidKey(std::string key);
		static bool	_isValueEmpty(std::vector<std::string> &vec);
		static bool	_checkAutoIndex(std::vector<std::string> &vec);
		static bool	_checkBodySize(std::vector<std::string> &vec);
		static bool	_checkPath(std::vector<std::string> &vec);
		static bool	_checkListen(std::vector<std::string> &vec);
		bool		_checkAllValue(map_type	&serverConfig);
		bool		_checkIpHost(void);


		
	public:
		Config(char *params);

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

