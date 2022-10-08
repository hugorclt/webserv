/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 11:32:35 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/08 16:45:04 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(std::map<std::string, std::vector<std::string>> reqData) : data(reqData) {}

HTTPRequest::~HTTPRequest() {}

/* -------------------------------------------------------------------------- */
/*                                   Methods                                  */
/* -------------------------------------------------------------------------- */

void	HTTPRequest::setData(std::map<std::string, std::vector<std::string>> map) {
	this->data = map;
}

std::map<std::string, std::vector<std::string>>	HTTPRequest::getData(void) const {
	return (this->data);
}
