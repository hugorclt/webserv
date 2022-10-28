/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:39:47 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/28 13:30:12 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include <vector>
#include "Request.hpp"
#define BUF_SIZE 64

class CgiHandler {
    private:
        std::vector<std::string>    _env;
        std::vector<std::string>    _var;

        char    **_convertVecToChar(std::vector<std::string> &vec);
        void    _initEnv(Request &req, std::string path, std::string MIMEtype, std::string clientIp, std::string root);
        std::string _constructQuery(std::vector<std::string> var);
        std::vector<char> 	_readPipe(int pipeToRead);
        void                _writeToIn(Request &req, int pipeIn);


      
        
    public:
        CgiHandler(Request &req, std::string path, std::string MIMEtype, std::string clientIp, std::vector<std::string> var, std::string root);
        std::vector<char>   exec(Request &req, std::string root, std::string binCgi);
};