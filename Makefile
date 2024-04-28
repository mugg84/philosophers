# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/11 12:38:07 by mmughedd          #+#    #+#              #
#    Updated: 2024/04/28 15:42:15 by mmughedd         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.c 

SRC_PATH = src/

UTILS = utils.c

UTILS_PATH = utils/

SRCS = $(addprefix $(SRC_PATH), $(SRC)) $(addprefix $(UTILS_PATH), $(UTILS))

INC = -I./include

OBJS = $(SRCS:.c=.o)

NAME = philo

CC = cc

RM = rm -f

FLAGS = -g #-fsanitize=thread -Wextra -Werror -Wall

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ $(INC)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(INC) -pthread

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

phony: all clean fclean re