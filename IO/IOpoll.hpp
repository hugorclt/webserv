/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOpoll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:56:52 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/18 11:26:08 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Servers.hpp"


class IOpoll {
	private:
		int epollfd;
		struct epoll_event ev;
		struct epoll_event *events;
		
	public:
		IOpoll(Servers servers);
		~IOpoll();

		int	getEpollfd(void) const;
		epoll_event	getEvent(void) const;
		epoll_event *getEvents(void) const;

		void	addFd(int fd);
};
