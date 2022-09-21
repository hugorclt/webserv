/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:18:44 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/21 15:18:51 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

char	*ft_strndup(char *str, int n) {
	char	*res;
	int		i;

	if (n == 0)
		return NULL;
	res = (char *)malloc(sizeof(char) * n + 1);
	for (i = 0; i < n; i++) {
		res[i] = str[i];
	}
	res[i] = '\0';
	return (res);
}
