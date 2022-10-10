/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:29 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/10 15:49:26 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(char *filename) {
	std::ifstream				input(filename);
	std::vector<std::string>	fullFile;

	for (std::string line; std::getline(input, line); ) {
		fullFile.push_back(line);
	}
	

	std::string::iterator	itStr = fullFile[0].begin();
	std::string::iterator	itStrEnd = fullFile[0].end();
	std::vector<std::string>::iterator	first = fullFile.begin();
	std::vector<std::string>::iterator	second = fullFile.end();

	std::string word1 = getWordSkipLine(itStr, itStrEnd, first, second);
	std::cout << word1 << std::endl;
	std::string word2 = getWordSkipSpace(itStr, itStrEnd);
	std::cout << word2 << std::endl;


	/*(for (std::vector<std::string>::iterator	it = fullFile.begin(); it != fullFile.end(); it++) {
		std::string::iterator	it_str = it->begin();
		std::cout << *it_str << std::endl;


		std::string serv = getNextWord();
		std::cout << serv << std::endl;
		if (serv == "server") {
			std::string word = getNextWord();
			if (*(word.begin()) == '{') {
				it_str = ++word.begin();
				_data.push_back(parseOneServ());
			}
			else {
				perror("error: parsing '{'");
				exit(EXIT_FAILURE);
			}
		}
		else {
			perror("error: parsing no \"server\"");
			exit(EXIT_FAILURE);
		}
	}*/
}
