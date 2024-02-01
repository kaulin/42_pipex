# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/31 15:00:04 by jajuntti          #+#    #+#              #
#    Updated: 2024/02/01 08:56:45 by jajuntti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_PATH = src/
OBJ_PATH = obj/

SRC = 
SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

LIBFT = libft.a 
LIBDIR = libft/
LIB_PATH = $(LIBDIR)$(LIBFT)

all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH):
	mkdir $(OBJ_PATH)
	
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(LIB_PATH): $(LIBFT)

$(LIBFT):
	make -C $(LIBDIR)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< $(LIB_PATH) -o $@ -I $(LIBDIR)

clean:
	rm -rf $(OBJ_PATH)
	make clean -C $(LIBDIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBDIR)

re: fclean all

.PHONY: all clean fclean re db