/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 11:47:56 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/31 15:25:12 by hrecolet         ###   ########.fr       */
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
		ConfigParser::Location	_env;
		Request				&_req;
		std::vector<std::string> _var;
		std::string			_clientIp;
		char				**_sysEnv;
		std::vector<char>	_data;
		std::string			_code;
		std::string			_status;
		std::string			_header;
		std::string			_types;
		std::ifstream		_file;

		/*static*/ std::map<std::string, std::string>			_mimeTypes;
		/*static*/ void											init_mimeTypes(void);
		/*static*/ std::map<std::string, void(Response::*)()>	_methodsFunction;
		/*static*/ void											init_methodsFunction(void);

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
		bool				_checkFile(std::string filename, int isErrorFile);
		void				_readPipe(int pipeToRead);
		std::string			_findCgiPath(std::string root);
		void				_uploadFile(void);
		void				_writeFile(void);


	public:
	
		Response(ConfigParser::Location env, Request &req, std::string clientIp, char **sysEnv);


		void	constructData(void);
		void	execute(void);
		void	sendData(int clientFd);
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
