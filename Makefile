# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/31 15:00:04 by jajuntti          #+#    #+#              #
#    Updated: 2024/02/11 14:32:16 by jajuntti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

SRC_DIR = src/
SRC =	main.c \
		pipex.c \
		pipex_utils.c
SRCS = $(addprefix $(SRC_DIR), $(SRC))

OBJ_DIR = obj/
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_DIR), $(OBJ))

LIB_DIR = libft/
LIBFT = libft.a
LIB_PATH = $(LIB_DIR)$(LIBFT)

INC = -I inc/ -I $(LIB_DIR)

all: $(NAME)
	
$(NAME): Makefile $(LIB_PATH) $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_PATH) -o $(NAME) $(INC)

$(LIB_PATH): $(LIBFT)

$(LIBFT):
	make -C $(LIB_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIB_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(LIB_PATH)

re: fclean all

.PHONY: all clean fclean re debug