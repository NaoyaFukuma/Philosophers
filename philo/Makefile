# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/18 22:41:02 by nfukuma           #+#    #+#              #
#    Updated: 2022/09/21 14:17:19 by nfukuma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# set up
NAME		=	philo
CC			=	gcc
CFLAGS		=	-Wall -Werror -Wextra

# for *.c set up
SRCS_NAME	=	main.c
SRCS_DIR	=	./srcs/
SRCS		=	$(addprefix $(SRCS_DIR), $(SRCS_NAME))

# for *.o set up
OBJ_DIR		=	./obj/
OBJS		=	$(SRCS_NAME:%.c=$(OBJ_DIR)%.o)

# heder include
INCLUDE_DIR	=	-I ./include/


# target rule
all : $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p obj

$(OBJS) : $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDE_DIR) -c -o $@ $<

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

