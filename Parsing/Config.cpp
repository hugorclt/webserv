/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/10 18:49:45 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

const std::string	Config::_option[N_OPT] {
	("listen"),
	("server_name"),
	("root"),
	("index"),
	("error"),
	("body_size"),
	("auto_index")
};

Config::Config(char *filename) {
	std::ifstream				input(filename);
	std::vector<std::string>	fullFile;

	for (std::string line; std::getline(input, line); ) {
		fullFile.push_back(line);
	}
	
	std::vector<std::string>::iterator first = fullFile.begin();
	std::string::iterator 				itStr = first->begin();
	std::string::iterator 				itStrEnd = first->end();
	std::vector<std::string>::iterator	second = fullFile.end();

	for (;first != fullFile.end();)
	{
		std::string word = _getWordSkipLine(itStr, itStrEnd, first, second);
		if (word == "server")
		{
			if (_getWordSkipLine(itStr, itStrEnd, first, second) == "{")
			{
				_data.push_back(parseOneServ(itStr, itStrEnd, first, second));
			}
			else
			{
				perror("error: parsing '{'");
				exit(EXIT_FAILURE);
			}
		}
		else if (!word.empty())
		{
			perror("error: parsing no \"server\"");
			exit(EXIT_FAILURE);
		}
	}
	int i = 1;
	for (data_type::iterator it = _data.begin(); it < _data.end(); it++) {
		std::cout << "Server : " << i << std::endl;
		printMap(*it);
		std::cout << std::endl;
		i++;
	}
}

bool	Config::_isValidKey(std::string key)
{
	for (int i = 0; i < N_OPT; i++)
	{
		if (_option[i] == key)
			return (true);
	}
	return (false);
}

void		Config::_skipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd)
{
	while (itStrBegin != itStrEnd && (*itStrBegin == '\t' || *itStrBegin == ' ')) {
		itStrBegin++;
	}
}

void	Config::_skipLineEmpty(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end)
{
	_skipSpace(itStrBegin, itStrEnd);
	if (first == end)
		return ;
	while (first != end && itStrBegin == itStrEnd) {
		first++;
		if (first == end)
			break ;
		itStrBegin = first->begin();
		itStrEnd = first->end();
		_skipSpace(itStrBegin, itStrEnd);
	}
}

std::string	Config::_getWord(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd)
{
	std::string::iterator	it = itStrBegin;
	
	while (it != itStrEnd && *it != ' ' && *it != '\t')
		it++;
	std::string res(itStrBegin, it);
	itStrBegin = it;
	return (res);
}

std::string	Config::_getWordSkipLine(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &end)
{
	_skipLineEmpty(itStrBegin, itStrEnd, first, end);
	std::string word = _getWord(itStrBegin, itStrEnd);
	_skipLineEmpty(itStrBegin, itStrEnd, first, end);
	return (word);
}

std::string	Config::_getWordSkipSpace(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd)
{
	_skipSpace(itStrBegin, itStrEnd);
	std::string word = _getWord(itStrBegin, itStrEnd);
	_skipSpace(itStrBegin, itStrEnd);
	return (word);
}

std::map<std::string, std::vector<std::string>>	Config::parseOneServ(std::string::iterator &itStrBegin, std::string::iterator &itStrEnd, std::vector<std::string>::iterator &first, std::vector<std::string>::iterator &last)
{
	std::map<std::string, std::vector<std::string>>	res;
	std::string word;
	
	while (first != last && *itStrBegin != '}')
	{
		std::string	key = _getWordSkipSpace(itStrBegin, itStrEnd);
		word = key;
		if ((key == "}" || key.empty()) || !_isValidKey(key))
			break ;
		std::vector<std::string>	value;
		while (itStrBegin != itStrEnd)
		{
			std::string wordValue = _getWordSkipSpace(itStrBegin, itStrEnd);
			word = wordValue;
			if (word == "}")
				break ;
			value.push_back(wordValue);
		}
		res.insert(std::make_pair(key, value));
		if (word == "}")
			break ;
		_skipLineEmpty(itStrBegin, itStrEnd, first, last);
		if (first == last)
			break ;
		itStrBegin = first->begin();
		itStrEnd = first->end();

	}
	if (word == "}")
		return (res);
	else
	{
		perror("parsing error: '}' not closed");
		exit(EXIT_FAILURE);
	}	
}
