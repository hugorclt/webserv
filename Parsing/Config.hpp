/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 11:36:05 by hrecolet          #+#    #+#             */
/*   Updated: 2022/10/10 10:54:30 by hrecolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"

class Config {
	public:
		typedef std::vector<std::map<std::string, std::vector<std::string>>> data_type;
	 
	private:
		int							_nbServer;
		data_type					_data;
		const static std::string	*option;
	
	public:
		Config(char *params);
};
