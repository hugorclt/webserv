/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 10:52:17 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/20 16:06:25 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>
#include <cstring>
#include <fcntl.h>
#include "Server.hpp"
#include "IOpoll.hpp"


#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define PORT 8080
#define MAX_EVENTS 5
