/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:50:45 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/10 15:30:36 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void		skipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd)
{
	while (itStrBegin != itStrEnd && (*itStrBegin == '\t' || *itStrBegin == ' ')) {
		itStrBegin++;
	}
}

void		skipLineEmpty(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end)
{
	skipSpace(itStrBegin, itStrEnd);
	while (first != end && itStrBegin == itStrEnd) {
		first++;
		itStrBegin = first->begin();
		itStrEnd = first->end();
		skipSpace(itStrBegin, itStrEnd);
	}
}

std::string	getWord(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd) {
	std::string::iterator	it = itStrBegin;
	
	while (it != itStrEnd && *it != ' ' && *it != '\t')
		it++;
	std::string res(itStrBegin, it);
	itStrBegin = it;
	return (res);
}

std::string	getWordSkipLine(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end)
{
	skipLineEmpty(itStrBegin, itStrEnd, first, end);
	std::string word = getWord(itStrBegin, itStrEnd);
	skipLineEmpty(itStrBegin, itStrEnd, first, end);
	return (word);
}

std::string	getWordSkipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd) {
	skipSpace(itStrBegin, itStrEnd);
	std::string word = getWord(itStrBegin, itStrEnd);
	skipSpace(itStrBegin, itStrEnd);
	return (word);
}

// std::map<std::string, std::vector<std::string>>	parseOneServ() {
// 	std::map<std::string, std::vector<std::string>>	res;

// 	while (it_str != it->end() && *it_str != '}') {
// 		std::string	key = getNextWord();
// 		std::vector<std::string>	value;
// 		while (it_str != it->end() && *it_str != '\n') {
// 			std::string word = getWord();
// 			if (*(--word.end()) == '}')
// 			{
// 				word.erase(--word.end());
// 				value.push_back(word);
// 				if (!res.insert(std::make_pair(key, value)).second) {
// 					perror("parsing error: doublon detected");
// 					exit(EXIT_FAILURE);
// 				}
// 				return (res);
// 			}
// 			value.push_back(word);
// 			skipSpace();
// 		}
// 		it++;
// 		it_str = it->begin();
// 		if (!res.insert(std::make_pair(key, value)).second) {
// 			perror("parsing error: doublon detected");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	return (res);
// }
