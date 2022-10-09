/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 13:59:42 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/09 16:42:20 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse(HTTPRequest &req, ServerList &servers) {
	std::map<std::string, std::vector<std::string>> dataReq = req.getData();
    std::string rootOfServer = servers.getRootOfServ(atoi(req.getData()["Host"][1].c_str()));
    

    execRequest(req , *this, rootOfServer);
	this->data.insert(std::make_pair("version", dataReq["httpVersion"][0]));
	this->data.insert(std::make_pair("date", getDate()));
	this->data.insert(std::make_pair("server", "webserv/1.0"));
    this->data.insert(std::make_pair("code", "200"));
    this->data.insert(std::make_pair("status", "OK"));
}

void     HTTPResponse::sendRequest(int clientFd) {
    std::string         header;

    header = this->data["version"] + " " + this->data["code"] + " " + this->data["status"] + "\r\n"
        + "Date: " + this->data["date"] + "\r\n"
        + "Server: " + this->data["server"] + "\r\n"
        + "Content-Length: " + to_string(this->body.size()) + "\r\n"
        // + "Content-Type: " + this->data["type"] + "\r\n"
        + "Content-Type: text/html\r\n" 
        + "Connection: Closed" + "\r\n"
        + "\n";

    this->body.insert(this->body.begin(), header.begin(), header.end());
    if (send(clientFd, this->body.data(), this->body.size(), 0) == -1)
        perror("send error:");
}

void    HTTPResponse::setBody(std::vector<char> body) {
    this->body = body;
}

void    HTTPResponse::setLength(int length) {
    this->length = length;
}