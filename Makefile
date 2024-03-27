# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jajuntti <jajuntti@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/31 15:00:04 by jajuntti          #+#    #+#              #
#    Updated: 2024/03/27 14:25:58 by jajuntti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address

SRC_DIR = src/
COMMON = pipex.c \
		child.c \
		utils.c \
		split_quote.c
SRC =	main.c $(COMMON)
SRCS = $(addprefix $(SRC_DIR), $(SRC))
BSRC =	main_bonus.c $(COMMON)
BSRCS = $(addprefix $(SRC_DIR), $(BSRC))

OBJ_DIR = obj/
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_DIR), $(OBJ))
BOBJ		= $(BSRC:.c=.o)
BOBJS	= $(addprefix $(OBJ_DIR), $(BOBJ))

LIB_DIR = libft/
LIBFT = libft.a
LIB_PATH = $(LIB_DIR)$(LIBFT)

INC = -I inc/ -I $(LIB_DIR)

all: $(NAME)
	
$(NAME): $(LIB_PATH) $(OBJ_DIR) $(OBJS) obj/.m
obj/.m: $(LIB_PATH) $(OBJ_DIR) $(OBJS)
	@touch obj/.m
	@rm -f obj/.b
	$(CC) $(CFLAGS) $(OBJS) $(LIB_PATH) -o $(NAME) $(INC)

bonus: $(LIB_PATH) $(OBJ_DIR) $(OBJS) $(BOBJS) obj/.b
obj/.b: $(LIB_PATH) $(OBJ_DIR) $(OBJS) $(BOBJS)
	@touch obj/.b
	@rm -f obj/.m
	$(CC) $(CFLAGS) $(BOBJS) $(LIB_PATH) -o $(NAME) $(INC)

$(LIB_PATH): $(LIBFT)

$(LIBFT):
	make -C $(LIB_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIB_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(LIB_PATH)

re: fclean all

.PHONY: all bonus clean fclean re