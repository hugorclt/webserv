/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:39:47 by hrecolet          #+#    #+#             */
/*   Updated: 2022/11/07 17:49:45 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include <vector>
#include "Request.hpp"
#include "Response.hpp"
#include "ConfigParser.hpp"

int	ft_strncmp(const char *s1, const char *s2, size_t n);

class Response;

#define BUF_SIZE 64

class CgiHandler {
    private:
		ConfigParser::Location		_server;
		Request						&_req;
		std::string					_cgiPath;
		std::string					_target;
		std::string					_pathToFile;
		std::string					_root;
		std::string					_type;
		std::string					_clientIp;
        std::vector<std::string>    _env;
        std::vector<std::string>    _var;
		char						**_sysEnv;
		Response					&_res;


        char    			**_convertVecToChar(std::vector<std::string> &vec);
        void    			_initEnv(void);
        std::string 		_constructQuery(std::vector<std::string> var);
        std::vector<char> 	_readPipe(int pipeToRead);
        int          	    _writeToIn(int pipeIn);
		void				_setCgiPath(void);
		void				_setPathToFile(void);
		void 				_setRoot(void);
		void				_setTarget(void);
		std::string			_findDirectory(std::string path);
		std::string			_getSysPath(void);
		std::string			_setCwd(void);

      
    public:
        CgiHandler(ConfigParser::Location &server, Request &req, std::string MIMEtype, std::string clientIp, char **env, Response &res);
       
	    int	exec(void);
		
		class CgiHandlerError: public std::exception {
			private:
				std::string	_error;
			public:
				CgiHandlerError(const std::string &error)
				: _error(error) {}

				virtual ~CgiHandlerError(void) throw() {}

				virtual const char *what() const throw()
				{ return (_error.c_str()); }
		};
};
