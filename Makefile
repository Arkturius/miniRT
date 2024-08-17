# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/06 21:19:50 by kiroussa          #+#    #+#              #
#    Updated: 2024/08/16 21:34:23 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#
# Vars
#

NAME			=	miniRT	
DEBUG			?=	0

BUILD_DIR		=	build

SRC				=	main.c					\
					mrtlib/mrt_mem.c		\
					mrtlib/mrt_str.c		\
					error/mrt_error.c		\
					mlx/mrt_mlx.c			\
					mlx/mrt_mlx_hooks.c		\
					parser/mrt_parser.c		\
					parser/mrt_line.c		\
					parser/mrt_pobjs.c		

SRC_DIR			=	src
SRC				:=	$(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR			=	$(BUILD_DIR)/obj
OBJ				=	$(subst $(SRC_DIR)/,,$(SRC:.c=.o))
OBJ				:=	$(addprefix $(OBJ_DIR)/, $(OBJ))

INCLUDE_DIR		= 	include

MLX_DIR			=	third-party/MacroLibX
MLX				=	$(MLX_DIR)/libmlx.so

CC				=	gcc
CFLAGS			= 	-Wall -Wextra -Werror -g3 -O3

COPTS			= 	-I $(INCLUDE_DIR) -I $(MLX_DIR)/$(INCLUDE_DIR)s
LDFLAGS			=	-lSDL2 -lm

ifeq ($(DEBUG), 1)
	CFLAGS		+=	-gdwarf-4 -g3
	LDFLAGS		+=	-rdynamic
endif

ifeq ($(LAZY), 1)
	CFLAGS		=	-Wall -Wextra -g3
endif

FILE			=	$(shell ls -lR $(SRC_DIR) | grep -F .c | wc -l)
CMP				=	1

RM				=	rm -rf

#
# Rules
#

all:		$(NAME)

$(NAME):	$(MLX) $(OBJ)
	@$(CC) $(CFLAGS) $(COPTS) -o $(NAME) $(OBJ) $(MLX) $(LDFLAGS)
	@printf "$(BOLD)   >$(GREEN) miniRT built !\n$(RESET)"

$(MLX):
	@make -C $(MLX_DIR) -s -j all

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo "$(BOLD)  🔩 building $(RESET)$(ITALIC)$@$(RESET)$(BOLD) from $(RESET)$(ITALIC)$^ ...$(RESET)"
	@$(CC) $(CFLAGS) $(COPTS) -c $< -o $@

remake:			oclean all

clean:			oclean
	@make -C $(MLX_DIR) clean

oclean:
	$(RM) $(OBJ_DIR) 

fclean:			clean
	@make -C $(MLX_DIR) fclean
	$(RM) $(NAME)

LAZY_SCENE	=	scenes/minimalist.rt
l:
	@make remake LAZY=1
	@./$(NAME) $(LAZY_SCENE)

VALGRIND_OPTS	=	--leak-check=full --track-origins=yes --show-leak-kinds=all 

ifeq ($(SUPP), 1)
	VALGRIND_OPTS	+=	--suppressions=third-party/MacroLibX/valgrind.supp
endif
	
vl:
	@make remake LAZY=1
	@valgrind $(VALGRIND_OPTS) ./$(NAME) $(LAZY_SCENE)

re:				fclean all

.PHONY:			all clean oclean fclean remake re l
.SILENT:		all clean oclean fclean remake re l

#
# Colors
#

BLACK		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
MAGENTA		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m

BOLD		=	\033[1m
ITALIC		=	\033[3m

RESET		=	\033[0m
LINE_CLR	=	\33[2K\r
