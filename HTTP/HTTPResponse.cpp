/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 13:59:42 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/08 18:12:35 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse(HTTPRequest &req) {
	std::map<std::string, std::vector<std::string>> dataReq = req.getData();
	
    execRequest(req ,this);
	this->data.insert(std::make_pair("version", dataReq["httpVersion"][0]));
	this->data.insert(std::make_pair("date", getDate()));
	this->data.insert(std::make_pair("server", "webserv/1.0"));
	this->data.insert(std::make_pair("length", this->length));
	this->data.insert(std::make_pair("type", this->type));
	this->data.insert(std::make_pair("body", this->body));
}

void     HTTPResponse::sendRequest(int clientFd) {
    std::string response;

    response = this->data["version"] + " " + this->data["code"] + " " + this->data["status"] + "\r\n"
        + "Date: " + this->data["date"] + "\r\n"
        + "Server: " + this->data["server"] + "\r\n"
        + "Content-Length: " + this->data["length"] + "\r\n"
        + "Content-Type: " + this->data["type"] + "\r\n"
        + "Connection: Closed" + "\r\n"
        + "body: " + this->data["body"];

    if (send(clientFd, response.c_str(), response.length(), 0) == -1)
    {
        perror("send error:");
    }
}
