/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 11:47:56 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/09 13:06:08 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class HTTPRequest;

class HTTPResponse {
	private:
		//headers
		std::map<std::string, std::string>	data;
		int									length;
		std::string							type;
		std::string							body;


	public:
		HTTPResponse(HTTPRequest &req, ServerList &servers);
		
		void    setBody(std::string body);
		void	setLength(int	length);
		void	sendRequest(int clientFd);
		
};

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
