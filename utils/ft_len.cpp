/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_len.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:19:04 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/22 18:47:09 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	getTabLen(char **tab) {
	int	size = 0;
	
	for (int i = 0; tab[i]; i++)
		size++;
	return (size);
}

int	getKeyLen(char *str) {
	int	size = 0;
	
	if (!str)
		return (0);
	while (str[size] && str[size] != ':') {
		size++;
	}
	return (size);
}

int	ft_strlen(const char *str) {
	int	i;

	i = 0;
	while (str[i]) {
		i++;
	}
	return (i);
}

