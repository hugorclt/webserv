/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 10:25:37 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/25 13:59:52 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class Request {
	public:
		typedef std::map< std::string, std::vector<std::string> > request_type;
		
	private:
		static const std::map<std::string, void(Request::*)(std::string &)> _splitTable;
		request_type				_header;
		std::string					_method;
		std::string					_target;
		std::string					_version;
		std::vector<char>			_body;
		std::vector<std::string>	_var;

		
		void	_basicSplit(std::string &line);
		void	_secSplit(std::string &line);
		void	_userSplit(std::string &line);
		void	_acceptSplit(std::string &line);
		void 	_parseFirstLine(std::vector<std::string> &request);
		void	_printValue(void);


	public:
		Request(std::string &req);
		~Request();

		request_type				getData(void) const;
		std::vector<std::string>	&getVar(void);
		std::string		getMethod(void) const;
		std::string		getTarget(void) const;
		std::string		getVersion(void) const;
};


/*
GET /index.html HTTP/1.1
Host: 127.0.0.1:8080
Connection: keep-alive
sec-ch-ua: "Google Chrome";v="105", "Not)A;Brand";v="8", "Chromium";v="105"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Linux"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*;q=0.8,application/signed-exchange;v=b3;q=0.9
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br
Accept-Language: en-US,en;q=0.9
*/

