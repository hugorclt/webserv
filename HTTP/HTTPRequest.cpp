/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 11:32:35 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/21 12:14:29 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(std::string methods, std::string target, std::string httpVersion) {
	this->_methods = methods;
	this->_target = target;
	this->_httpVersion = httpVersion;
}

HTTPRequest::~HTTPRequest() {
	
}

/* -------------------------------------------------------------------------- */
/*                                   Methods                                  */
/* -------------------------------------------------------------------------- */


