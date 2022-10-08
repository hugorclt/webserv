/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 13:59:42 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/08 14:18:24 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse(HTTPRequest &req) {
    
}

void     HTTPResponse::sendRequest(int clientFd) {
    std::string response;

    response = this->data["version"] + " " + this->data["code"] + " " + this->data["status"] + "\r\n"
        + "Date: " + this->data["data"] + "\r\n"
        + "Server: " + this->data["server"] + "\r\n"
        + "Last-Modified: " + this->data["lastModified"] + "\r\n"
        + "Content-Length: " + this->data["length"] + "\r\n"
        + "Content-Type: " + this->data["type"] + "\r\n"
        + "Connection: " + this->data["connection"] + "\r\n"
        + "body: " + this->data["body"];

    if (send(clientFd, response.c_str(), response.length(), 0) == -1)
    {
        perror("send error:");
    }
}