# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/19 10:51:56 by hrecolet          #+#    #+#              #
#    Updated: 2022/10/17 14:23:21 by hrecolet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		=	main.cpp\
				Parsing/ConfigParser.cpp\
				Server/Server.cpp\
				IO/IOpoll.cpp\
				Parsing/parse.cpp\
				utils/ft_split.cpp\
				Server/ServerList.cpp\
				Parsing/parse_config.cpp\
				utils/copyFile.cpp\
				exec/execRequest.cpp\
				utils/intToStr.cpp\
				utils/isDigits.cpp


INCL		=	includes/webserv.hpp\
				includes/includes.hpp\
				Parsing/ConfigParser.hpp\
				Server/Server.hpp\
				IO/IOpoll.hpp\
				Server/ServerList.hpp

CC			=	c++

CPPFLAGS		=	-Wall -Werror -Wextra -std=c++98 -g3

RM			=	rm -f

NAME		=	webserv

OBJ			=	$(SRCS:.cpp=.o)

all			:	$(NAME) 

%.o : %.cpp
	$(CC) $(CFLAGS) -I./Parsing -I./includes -I./HTTP -I./IO -I./Server -o $@ -c $< 

$(NAME)		:	$(OBJ) $(INCL)
				$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME) 
		
clean		:	
				$(RM) $(OBJ)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	clean fclean re all
