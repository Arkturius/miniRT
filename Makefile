# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/06 21:19:50 by kiroussa          #+#    #+#              #
#    Updated: 2024/09/13 14:55:01 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#
# Vars
#

NAME			=	miniRT	
DEBUG			?=	0
FAST			?=	0

CC				=	gcc
CFLAGS			= 	-Wall -Wextra -Werror

COPTS			= 	-I $(INCLUDE_DIR) -I $(MLX_DIR)/$(INCLUDE_DIR)s
LDFLAGS			=	-lSDL2 -lm

ifeq ($(FAST), 1)
	CFLAGS		+=	-O3
endif

ifeq ($(DEBUG), 1)
	CFLAGS		+=	-gdwarf-4 -g3
	LDFLAGS		+=	-rdynamic
endif

ifeq ($(LAZY), 1)
	CFLAGS		=	-Wall -Wextra
endif

SRC				=	main.c						\
					mrtlib/mrt_mem.c			\
					mrtlib/mrt_str.c			\
					mrtlib/mrt_chr.c			\
					mrtlib/mrt_vec.c			\
					mrtlib/mrt_vec2.c			\
					mrtlib/mrt_math.c			\
					mrtlib/mrt_tox.c			\
					mrtlib/mrt_list.c			\
					mrtlib/mrt_io.c				\
					mrtlib/mrt_io_save.c		\
					mrtlib/mrt_io_load.c		\
					error/mrt_error.c			\
					mlx/mrt_mlx.c				\
					mlx/mrt_mlx_hooks.c			\
					parser/mrt_parse_file.c		\
					parser/mrt_parse_objs.c		\
					parser/mrt_parse_prim.c		\
					parser/mrt_line.c			\
					objects/mrt_aabb.c			\
					objects/mrt_obj_chunk.c		\
					render/mrt_scene.c			\
					render/mrt_scene_render.c	\
					render/mrt_rays.c			\
					render/mrt_objects.c		\
					render/mrt_colors.c			\
					save/mrt_scene_file.c

ifeq ($(MRT_BONUS), 1)
	CFLAGS		+=	-DMRT_BONUS -mavx2
	SRC			+=	parser/mrt_parse_objs_bonus.c	\
					parser/mrt_parse_objf.c
endif

SRC_DIR			=	src
SRC				:=	$(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR			=	.obj
OBJ				=	$(subst $(SRC_DIR)/,,$(SRC:.c=.o))
OBJ				:=	$(addprefix $(OBJ_DIR)/, $(OBJ))

INCLUDE_DIR		= 	include

MLX_DIR			=	third-party/MacroLibX
MLX				=	$(MLX_DIR)/libmlx.so

FILE			=	$(shell ls -lR $(SRC_DIR) | grep -F .c | wc -l)
CMP				=	1

RM				=	rm -rf

#
# Rules
#

all:		art $(NAME)

art:
	echo -e $(MINIRT_ART)

bonus:
	make --no-print-directory remake MRT_BONUS=1

$(NAME):	$(MLX) $(OBJ)
	@$(CC) $(CFLAGS) $(COPTS) -o $(NAME) $(OBJ) $(MLX) $(LDFLAGS)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  building	$(GREEN)$(BOLD)$(ITALIC)$(NAME)$(RESET)"

$(MLX):
	@make --no-print-directory -C $(MLX_DIR) -s -j all

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$^$(RESET)"
	@$(CC) $(CFLAGS) $(COPTS) -c $< -o $@

remake:		oclean all

oclean:		art
	echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(OBJ_DIR)$(RESET)"
	$(RM) $(OBJ_DIR)

clean:		oclean
	echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  cleaned	$(RED)$(BOLD)$(ITALIC)$(MLX_DIR)$(RESET)"
	make --no-print-directory -C $(MLX_DIR) clean

fclean:		clean
	make --no-print-directory -C $(MLX_DIR) fclean
	echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(NAME)$(RESET)"
	$(RM) $(NAME)

LAZY_SCENE	=	scenes/minimalist.rt
l:
	make remake LAZY=1
	./$(NAME) $(LAZY_SCENE)

VALGRIND_OPTS	=	--leak-check=full --track-origins=yes --show-leak-kinds=all 

ifeq ($(SUPP), 1)
	VALGRIND_OPTS	+=	--suppressions=third-party/MacroLibX/valgrind.supp
endif
	
vl:
	@make remake LAZY=1
	@valgrind $(VALGRIND_OPTS) ./$(NAME) $(LAZY_SCENE)

re:				fclean all

.PHONY:			art all bonus clean oclean fclean remake re l vl
.SILENT:		art all bonus clean oclean fclean remake re l vl

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
GRAY		=	\033[90m

BOLD		=	\033[1m
ITALIC		=	\033[3m

RESET		=	\033[0m
LINE_CLR	=	\33[2K\r

MINIRT_ART	=	'$(BOLD)$(CYAN)              _       _   ____    ______\n   ____ ___  (_)___  (_) / __ \  /_  __/\n  / __ `__ \/ / __ \/ / / /_/ /   / /   \n / / / / / / / / / / / / _, _/   / /    \n/_/ /_/ /_/_/_/ /_/_/ /_/ |_|ay /_/racer\n$(RESET)'
