# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kpoilly <kpoilly@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/29 13:43:51 by kpoilly           #+#    #+#              #
#    Updated: 2024/10/29 15:21:46 by kpoilly          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 			= ircserv

HEAD			= ./includes/ft_irc.hpp ./includes/utils.hpp \
				  ./includes/server.hpp ./includes/channel.hpp ./includes/user.hpp

M_FILES 		= main.cpp utils.cpp
M_SRCS			= $(addprefix ./objs/, $(M_FILES))
M_OBJECTS 		= $(M_SRCS:%.cpp=%.o)

B_FILES 		= 
B_SRCS			= $(addprefix ./objs/bonus/, $(B_FILES))
B_OBJECTS 		= $(B_SRCS:%.cpp=%.o)

RM 						= @rm -f
CC 						= c++
CFLAGS 			= -Wall -Wextra -Werror -MMD -std=c++98

all:			$(NAME)

.cpp.o:
				@$(CC) $(CFLAGS) -c $< -o ${<:.cpp=.o}

./objs/%.o:./srcs/%.cpp
				@mkdir -p ./objs
				@$(CC) $(CFLAGS) -c $< -o $@
				
$(NAME):		$(M_OBJECTS) $(B_OBJECTS) $(HEAD)
				@$(CC) $(CFLAGS) -I ./ -o $(NAME) $(M_OBJECTS) $(B_OBJECTS)
				@echo "\033c\033[0;32mMandatory part compiled.\033[0;37m"

bonus:			fclean $(M_OBJECTS) $(B_OBJECTS) $(HEAD)
						@$(CC) $(CFLAGS) -I ./includes/ -o $(NAME) $(M_OBJECTS) $(B_OBJECTS) \
				@echo "\033c\033[0;32mBonus part compiled.\033[0;37m"
							
clean:
						$(RM) $(M_OBJECTS) $(B_OBJECTS)
						@echo "\033[0;33m🧽Cleaned🫧\033[0;37m"

fclean:			clean
						$(RM) -r ./objs
						$(RM) $(NAME)
re:						fclean all
.PHONY:		all clean fclean re