/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_value.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:21:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/09/22 18:45:59 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	skipFirstWord(char *line) {
	int	i = 0;
	
	while (line[i] && line[i] != ':') {
		i++;
	}
	i++;
	return (i);
}

int	is_sep(char c, int i, char *str) {
	int res = 0;
	int count_parenthesis = 0;
	int	count_quote = 0;

	for (int j = 0; j < i; j++) {
		if (str[j] == '(')
			count_parenthesis++;
		else if (str[j] == ')')
			count_parenthesis--;
		if (str[j] == '"')
			count_quote++;
	}
	if (count_parenthesis <= 0 && count_quote % 2 == 0 && (c == ' ' || c == ','))
		return (TRUE);
	else
		return (FALSE);
}

int	find_len_word(char *str, int i) {
	int res = 0;

	while (str[i] && is_sep(str[i], i, str) == FALSE) {
		i++;
		res++;
	}
	return (res);
}

int	ft_count_word(char *line, int i) {
	int	res = 0;
	
	while (line[i])
	{
		while (line[i] && is_sep(line[i], i, line) == TRUE) {
			i++;
			if (!line[i])
				return (res);
		}
		res++;
		while (line[i] && is_sep(line[i], i, line) == FALSE) {
			i++;
			if (!line[i])
				return (res);
		}
		i++;
	}
	return (res);
}

char	**ft_split_value(char *line) {
	int		i = skipFirstWord(line);
	int		len_word = 0;
	int		iter;
	char	**tab;

	tab = (char **)malloc(sizeof(char *) * (ft_count_word(line, i) + 1));
	if (!tab)
		return (NULL);
	while (line[i]) {
		while (line[i] && is_sep(line[i], i, line) == TRUE) {
			i++;
			if (!line[i])
				return (tab);
		}
		len_word = find_len_word(line, i);
		tab[iter] = ft_substr(line, i, len_word);
		i += len_word;
		iter++;
	}
	tab[iter] = NULL;
	return (tab);
}


