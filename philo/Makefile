# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/18 22:41:02 by nfukuma           #+#    #+#              #
#    Updated: 2022/10/07 02:39:55 by nfukuma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# set up
NAME		=	philo
CC		=	gcc
CFLAGS		=	-g -Wall -Werror -Wextra
#CFLAGS		=	-g -Wall -Werror -Wextra -fsanitize=thread

# for *.c set up
SRCS_NAME	=	main.c each_philo_routine.c moni_philos_routine.c util.c
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

$(OBJ_DIR)%.o : $(SRCS_DIR)%.c
	$(CC) $(CFLAGS) $(INCLUDE_DIR) -c -o $@ $<

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

