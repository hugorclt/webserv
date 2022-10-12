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

struct ServerConfig
{
	typedef std::map<std::string, std::vector<std::string>>							confType;
	typedef std::map<std::string, std::map<std::string, std::vector<std::string>>>	locationType;

	confType		conf;
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
		typedef std::vector<ServerConfig> 	data_type;
		typedef std::map<std::string, std::vector<std::string>>	map_type;
		
	private:
		int							_nbServer;
		data_type					_data;
		const static std::pair<std::string, bool(*)(std::vector<std::string> &)>	_option[N_OPT];
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
		ServerConfig::confType										_createNewLocation(lineRange_type &strIt, fileRange_type &fileIt);
		keyValues_type			_getKeyValuePair(lineRange_type &strIt);

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

