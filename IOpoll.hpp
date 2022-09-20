/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOpoll.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 15:56:52 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/20 16:22:53 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class IOpoll {
	private:
		int epollfd;
		struct epoll_event ev;
		struct epoll_event events[5];
		
	public:
		IOpoll();
		~IOpoll();

		int	getEpollfd(void) const;
		epoll_event	getEvent(void) const;
		epoll_event getEvents(void) const;

		void	addFd(int fd);
		void	waitForchange(void);
};
