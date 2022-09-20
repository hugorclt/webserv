/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOpoll.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:57:26 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/20 17:50:07 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IOpoll.hpp"

IOpoll::IOpoll(void) {
	this->ev.events = EPOLLIN | EPOLLET;
	this->events = (epoll_event *)malloc(sizeof(epoll_event) * MAX_EVENTS);
	this->epollfd = epoll_create1(0);
	if (this->epollfd < 0)
	{
		perror("Epoll creation failure");
		exit(EXIT_FAILURE);
	}
}

IOpoll::~IOpoll(void) {
	
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
