/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 10:52:17 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/19 11:22:07 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>


#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define PORT 8080
