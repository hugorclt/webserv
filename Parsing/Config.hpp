/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:05 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/10 18:35:23 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

#define N_OPT 7

class Config {
	public:
		typedef std::vector<std::map<std::string, std::vector<std::string>>> data_type;
	 
	private:
		int							_nbServer;
		data_type					_data;
		const static std::string	_option[N_OPT];
	
		//parseFunction
		std::map<std::string, std::vector<std::string>>	parseOneServ(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end);
		std::string										_getWordSkipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd);
		std::string										_getWordSkipLine(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end);
		std::string										_getWord(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd);
		void											_skipLineEmpty(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end);
		void											_skipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd);

		//Check Functions
		bool	_isValidKey(std::string key);
		
	public:
		Config(char *params);
};
