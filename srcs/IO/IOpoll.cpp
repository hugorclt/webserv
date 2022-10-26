/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOpoll.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:57:26 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/26 13:43:47 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IOpoll.hpp"

IOpoll::IOpoll(Servers servers) {
	this->ev.events = EPOLLIN;
	this->events = new epoll_event[1];
	this->epollfd = epoll_create1(0);
	this->ev.data.ptr = NULL;
	
	if (this->epollfd < 0)
	{
		perror("Epoll creation failure");
		exit(EXIT_FAILURE);
	}

	Servers::sock_type serv = servers.getSockIpPort();
	for (Servers::sock_type::iterator it = serv.begin(); it != serv.end(); it++) {
		ev.data.fd = it->first;
		if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, it->first, &this->ev)) {
			perror("Failed to add fd to epoll list");
			close(this->epollfd);
			exit(EXIT_FAILURE);
		}
	}
}

IOpoll::~IOpoll(void) {
	delete []events;
}

/* -------------------------------------------------------------------------- */
/*                                  Accessor                                  */
/* -------------------------------------------------------------------------- */

int	IOpoll::getEpollfd(void) const{
	return (this->epollfd);
}

epoll_event	IOpoll::getEvent(void) const{
	return (this->ev);
}

epoll_event	*IOpoll::getEvents(void) const{
	return (this->events);
}

/* -------------------------------------------------------------------------- */
/*                                   methods                                  */
/* -------------------------------------------------------------------------- */

void	IOpoll::addFd(int fd) {
	ev.data.fd = fd;
	if (epoll_ctl(this->epollfd, EPOLL_CTL_ADD, fd, &this->ev)) {
		perror("Failed to add fd to epoll list");
		close(this->epollfd);
		exit(EXIT_FAILURE);
	}
}
