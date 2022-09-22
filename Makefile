# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hrecolet <hrecolet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/19 10:51:56 by hrecolet          #+#    #+#              #
#    Updated: 2022/09/22 18:30:19 by hrecolet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		=	main.cpp\
				Server/Server.cpp\
				IO/IOpoll.cpp\
				HTTP/HTTPRequest.cpp\
				HTTP/HTTPResponse.cpp\
				utils/ft_split.cpp\
				utils/ft_free.cpp\
				utils/ft_substr.cpp\
				utils/ft_len.cpp\
				utils/ft_strndup.cpp\
				Parsing/parse.cpp\
				Parsing/split_value.cpp\

INCL		=	includes/webserv.hpp\
				Server/Server.hpp\
				IO/IOpoll.hpp\
				HTTP/HTTPRequest.hpp\
				HTTP/HTTPResponse.hpp

CC			=	c++

CPPFLAGS		=	-Wall -Werror -Wextra -std=c++98 -g3

RM			=	rm -f

NAME		=	webserv

OBJ			=	$(SRCS:.cpp=.o)

all			:	$(NAME) 

%.o : %.cpp
	$(CC) $(CFLAGS) -I./includes -I./HTTP -I./IO -I./Server -o $@ -c $< 

$(NAME)		:	$(OBJ) $(INCL)
				$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME) 
		
clean		:	
				$(RM) $(OBJ)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	clean fclean re all
