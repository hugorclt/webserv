/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 11:47:56 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/07 18:29:20 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"
#include "ConfigParser.hpp"
#include "CgiHandler.hpp"
#include <fstream>

#define NB_MIME 66

class Response {
	private:
		ConfigParser::Location		_env;
		Request						&_req;
		std::vector<std::string>	_var;
		std::string					_clientIp;
		char						**_sysEnv;
		std::vector<char>			_data;
		std::string					_code;
		std::string					_status;
		std::string					_header;
		std::string					_types;
		std::ifstream				_file;

		std::map<std::string, std::string>			_mimeTypes;
		void										init_mimeTypes(void);
		std::map<std::string, void(Response::*)()>	_methodsFunction;
		void										init_methodsFunction(void);

		std::string	_getDate(void);
		std::vector<char>	_getDefaultErrorPage(void);
		void				_setType(std::string url);
		bool				_isBinaryFile(std::string filePath);
		void				_readFile(std::ifstream &file);
		bool				_isFileAccessible(std::string filename);
		void				_execGet(void);
		void				_execDel(void);
		bool				_isCgiFile(std::string root);
		int					_execCgi(std::string root);
		void				_setError(std::string code);
		bool				_checkFile(std::string filename);
		void				_readPipe(int pipeToRead);
		std::string			_findCgiPath(std::string root);
		void				_uploadFile(void);
		void				_writeFile(void);


	public:
		const static std::string	_css;
		Response(ConfigParser::Location env, Request &req, std::string clientIp, char **sysEnv);


		void	constructData(void);
		void	execute(void);
		void	sendData(int clientFd);
		void	setData(std::vector<char> data);

		class ResponseError: public std::exception {
			private:
				std::string	_error;
			public:
				ResponseError(const std::string &error)
				: _error(error) {}

				virtual ~ResponseError(void) throw() {}

				virtual const char *what() const throw()
				{ return (_error.c_str()); }
		};
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
		HTTPResponse(Request &req, ServerList &servers);
		
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
