/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 11:47:56 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/18 17:21:04 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"
#include "HTTPRequest.hpp"
#include "ConfigParser.hpp"

#define NB_MIME 66

class Response {
	private:
		HTTPRequest				&_req;
		ConfigParser::Location	&_env;
		std::vector<char>	_data;
		std::string			_code;
		std::string			_status;
		std::string			_header;
		std::string			_types;

		const static std::map<std::string, std::string>	_mimeTypes;
		const static std::map<std::string, void(Response::*)()> _methodsFunction;
		void		_constructBody(void);
		void		_constructHeader(void);
		std::string	_getDate(void);
		std::string	_getDefaultErrorPage(void);
		void		_setType(std::string url);
		bool		_isBinaryFile(std::string filePath);
		void		_readFile(std::ifstream file);
		bool		is_file_opened(std::ifstream file);
		bool		is_file_accessible(std::string filename);
		void		_execGet(void);





	public:
	
		Response(ConfigParser::Location &env, HTTPRequest &req);


		void	execute(void);
		void	send(int clientFd);
};












/*
class HTTPResponse {
	private:
		//headers
		std::map<std::string, std::string>	data;
		int									length;
		std::string							type;
		std::vector<char>					body;


	public:
		HTTPResponse(HTTPRequest &req, ServerList &servers);
		
		void    setBody(std::vector<char> body);
		void	setLength(int	length);
		void	sendRequest(int clientFd);
		
};
*/
/*
HTTP/1.1 200 OK
Date: Mon, 27 Jul 2009 12:28:53 GMT
Server: Apache/2.2.14 (Win32)
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
Content-Length: 88
Content-Type: text/html
Connection: Closed
*/

/*
HTTP/1.1 404 Not Found
Date: Sun, 18 Oct 2012 10:36:20 GMT
Server: Apache/2.2.14 (Win32)
Content-Length: 230
Connection: Closed
Content-Type: text/html; charset=iso-8859-1

*/
