# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmughedd <mmughedd@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/11 12:38:07 by mmughedd          #+#    #+#              #
#    Updated: 2024/05/06 12:18:29 by mmughedd         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BONUS = main_b.c actions.c bools_set.c time.c utils.c utils2.c setters.c

BONUS_PATH = bonus/

BNS = $(addprefix $(BONUS_PATH), $(BONUS))

INC_B = -I./include_b

OBJS_B = $(BNS:.c=.o)

NAME_B = philo_bonus

CC = cc

RM = rm -f

FLAGS = -g -Wextra -Werror -Wall

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ $(INC_B)

bonus: $(OBJS_B)
	$(CC) $(FLAGS) -o $(NAME_B) $(OBJS_B) $(INC_B) -pthread -lrt
	
clean:
	$(RM) $(OBJS_B)

fclean: clean
	$(RM) $(NAME_B)

re: fclean bonus

phony: bonus clean fclean re