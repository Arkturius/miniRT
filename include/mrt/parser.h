// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   parser.h                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/09/14 13:02:38 by rgramati          #+#    #+#             //
//   Updated: 2024/09/17 15:28:24 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PARSER_H
# define PARSER_H

# include <mrtlib.h>
# include <mrt/error.h>
#include <stdint.h>

/* PARSED OBJS HOLDERS ****************************************************** */

# ifndef MRT_BONUS

typedef enum e_mrt_objects
{
	MRT_OBJ_NONE,
	MRT_OBJ_AMBIENT,
	MRT_OBJ_CAMERA,
	MRT_OBJ_LIGHT,
	MRT_OBJ_SPHERE,
	MRT_OBJ_PLANE,
	MRT_OBJ_CYLINDER
}	t_objtype;

# else

typedef enum e_mrt_objects
{
	MRT_OBJ_NONE,
	MRT_OBJ_AMBIENT,
	MRT_OBJ_CAMERA,
	MRT_OBJ_LIGHT,
	MRT_OBJ_SPHERE,
	MRT_OBJ_PLANE,
	MRT_OBJ_CYLINDER,
	MRT_OBJ_OBJFILE
}	t_objtype;

# endif

/* LINES ******************************************************************** */

# define MRT_LINE_LEN 248

typedef struct s_line
{
	struct s_line	*next;
	char			str[MRT_LINE_LEN];
}	t_line;

t_line
*mrt_line_new(const char *str, t_bool copy, uint32_t size);

void
mrt_line_clean(t_line *list);

void
mrt_line_copy(t_line *line, const char *str, uint32_t size);

void
mrt_line_push(t_line **list, t_line *new);

uint32_t
mrt_line_count(t_line *list);

/* PARSER ******************************************************************* */

typedef enum e_filetype
{
	MRT_FILE,
	MRT_FILE_RT,
	MRT_FILE_CUB,
	MRT_FILE_OBJ,
}	t_filetype;

typedef struct s_mrt_file
{
	const char	*filename;
	int32_t		fd;
	t_filetype	type;
	char		*data;
}	t_file;

t_error
mrt_parse_file(const char *name, t_file *file);

t_errtype
mrt_parse_int(int *res, char *str, char **remain);

t_errtype
mrt_parse_float(float *res, char *str, char **remain);

t_errtype
mrt_parse_vec(t_mrt_vec *res, char *str, char **remain);

t_errtype
mrt_parse_color(t_mrt_color *res, char *str, char **remain);

# define MRT_FORMAT			"ifcv"
# define MRT_FMT_AMBIENT	"56 !f 48 !c"
# define MRT_FMT_CAMERA		"16 !v 32 !v 8 !i"
# define MRT_FMT_LIGHT		"16 !v 56 !f 48 !c"
# define MRT_FMT_SPHERE		"16 !v 8 !f 48 !c"
# define MRT_FMT_PLANE		"16 !v 32 !v 48 !c"
# define MRT_FMT_CYLINDER	"16 !v 32 !v 8 !f 12 !f 48 !c"

typedef struct s_mrt_scene	t_scene;

t_error
mrt_parse_obj(t_scene *scene, char *str, char **remain);

t_error
mrt_parse_obj_format(uint8_t *ptr, char *fmt, char *str, char **remain);

t_errtype
mrt_parse_obj_value(uint8_t *ptr, char *fmt, char *str, char **remain);

# ifdef MRT_BONUS

t_error
mrt_parse_obj_bonus(t_scene *scene, char *str, char **remain);

t_error
mrt_parse_obj_file(t_scene *scene, uint8_t *ptr, char *str, char **remain);

# endif

#endif	// PARSER_H
