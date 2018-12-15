# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/19 14:03:15 by jgourdin          #+#    #+#              #
#    Updated: 2018/12/14 18:34:17 by gmichaud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

LINKNAME = libft_malloc.so

INC_FLAGS = -I./includes -I./libft/includes

INC_NAME = lib_malloc.h

INC_PATH = ./includes

LIB_FLAGS = -L./libft

LIBS = -lft

CC = clang

CFLAGS = -Wall -Wextra -Werror
SRC_PATH = src

SRC_NAME = malloc.c free.c realloc.c block.c size.c zone.c free_list1.c \
			free_list2.c zone_list1.c zone_list2.c debug1.c debug2.c

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))

OBJ_PATH = obj

OBJ_NAME = $(SRC:.c=.o)

OBJ = $(subst $(SRC_PATH),$(OBJ_PATH),$(OBJ_NAME))

INC = $(addprefix $(INC_PATH)/,$(INC_NAME))

GREEN = \033[32m

all: $(NAME)

$(NAME): $(OBJ)
	@make -C ./libft --no-print-directory
	@$(CC) -shared -o $@ $(OBJ) $(LIB_FLAGS) $(LIBS)
	@ln -s $@ $(LINKNAME) 2> /dev/null || true
	@echo "$(GREEN)[$(NAME)] Compilation success"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INC)
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) $(CFLAGS) $(INC_FLAGS) -o $@ -c $< 

clean:
	@/bin/rm -f $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@make -C ./libft clean --no-print-directory
	@echo "$(GREEN)[$(NAME)] .o files deleted"

fclean: clean
	@/bin/rm -f $(NAME)
	@/bin/rm -f $(LINKNAME)
	@make -C ./libft fclean --no-print-directory
	@echo  "$(GREEN)[$(NAME)] executable file deleted"
	@/bin/rm -f ./minilibx_macos/libmlx.a

re: fclean $(NAME)

.PHONY: all, clean, fclean, re
