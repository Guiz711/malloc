# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmichaud <gmichaud@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/19 14:03:15 by jgourdin          #+#    #+#              #
#    Updated: 2018/11/30 15:22:52 by gmichaud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

LINKNAME = libft_malloc.so

INC_FLAGS = -I./includes -I./libft/includes

LIB_FLAGS = -L./libft

LIBS = -lft

CC = clang

CFLAGS = -Wall -Wextra -Werror
SRC_PATH = src

SRC_NAME = malloc.c free.c realloc.c zone.c lst_handling.c free_list.c \
			debug1.c debug2.c

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))

OBJ_PATH = obj

OBJ_NAME = $(SRC:.c=.o)

OBJ = $(subst $(SRC_PATH),$(OBJ_PATH),$(OBJ_NAME))

GREEN = \033[32m

all: $(NAME)

$(NAME): compilation_end
	@make -C ./libft --no-print-directory
	@$(CC) -shared -o $@ $(OBJ) $(LIB_FLAGS) $(LIBS)
	ln -s $@ $(LINKNAME)
	@echo "$(GREEN)[$(NAME)] Compilation success"

compilation_end: $(OBJ)
	@echo "$(GREEN)[$(NAME)] .o created"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
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
