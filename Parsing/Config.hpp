/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:05 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/11 11:22:28 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

#define N_OPT		7
#define MAX_PORT	65535

class Config {
	public:
		typedef std::vector<std::map<std::string, std::vector<std::string>>> 	data_type;
		typedef std::map<std::string, std::vector<std::string>>					map_type;
		
	private:
		int							_nbServer;
		data_type					_data;
		const static std::pair<std::string, bool(*)(std::vector<std::string> &)>	_option[N_OPT];
	
		//parseFunction
		map_type	_parseOneServ(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end);
		std::string	_getWordSkipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd);
		std::string	_getWordSkipLine(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end);
		std::string	_getWord(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd);
		void		_skipLineEmpty(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end);
		void		_skipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd);
		std::pair<std::string, bool(*)(std::vector<std::string> &)>	_getOpt(std::string key);



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

