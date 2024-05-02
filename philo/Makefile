# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/11 12:38:07 by mmughedd          #+#    #+#              #
#    Updated: 2024/05/02 06:12:15 by mmughedd         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.c time.c actions.c bools.c philo.c setters.c utils.c write.c

SRC_PATH = src/

SRCS = $(addprefix $(SRC_PATH), $(SRC))

INC = -I./include

OBJS = $(SRCS:.c=.o)

NAME = philo

CC = cc

RM = rm -f

FLAGS = -g -Wextra -Werror -Wall #-fsanitize=thread

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