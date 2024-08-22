// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   parser.h                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/13 20:08:34 by rgramati          #+#    #+#             //
//   Updated: 2024/08/22 22:40:25 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PARSER_H
# define PARSER_H

# include <mrtlib.h>

typedef enum e_mrt_error		t_error;

// PARSED OBJS HOLDERS ****************************************************** //

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

typedef struct s_pobj_header	t_pheader;

typedef t_error	(mrt_parse_pobj_func)(t_pheader **, char *, char **);

struct	s_pobj_header
{
	t_pheader	*next;
	t_u32		obj_type;
};

typedef struct s_pobj_gen	t_pobj_gen;

struct	s_pobj_gen
{
	t_pheader	header;
	union
	{
		t_s32		ints[12];
		t_f32		floats[12];
		t_mrt_color	colors[12];
		t_mrt_vec	vecs[3];
	};
};

#define MRT_PFORMAT				"ifvc"
#define MRT_PFORMAT_AMBIENT		"!f !c"
#define MRT_PFORMAT_CAMERA		"!v !v !i"
#define MRT_PFORMAT_LIGHT		"!v !f !c"
#define MRT_PFORMAT_SPHERE		"!v !f !c"
#define MRT_PFORMAT_PLANE		"!v !v !c"
#define MRT_PFORMAT_CYLINDER	"!v !v !f !f !c"

typedef struct s_parsed_ambient	// |HHHH HHHH HHHH ....|RRRR|CCCC
{
	t_pheader	header;
	t_f32		ratio;
	t_mrt_color	color;
}	t_pobj_ambient;

typedef struct s_parsed_camera	// |HHHH HHHH HHHH ....|VVVV VVVV VVVV VVVV|OOOO OOOO OOOO OOOO|FFFF|....
{
	t_pheader	header;
	t_mrt_vec	viewpoint;
	t_mrt_vec	orientation;
	t_s32		fov;
}	t_pobj_camera;

typedef struct s_parsed_light	// |HHHH HHHH HHHH ....|LLLL LLLL LLLL LLLL|RRRR|CCCC
{
	t_pheader	header;
	t_mrt_vec	lightpoint;
	t_f32		ratio;
	t_mrt_color	color;
}	t_pobj_light;

typedef struct s_parsed_sphere	// |HHHH HHHH HHHH ....|CCCC CCCC CCCC CCCC|DDDD|CCCC
{
	t_pheader	header;
	t_mrt_vec	center;
	t_f32		diameter;
	t_mrt_color	color;
}	t_pobj_sphere;

typedef struct s_parsed_plane	// |HHHH HHHH HHHH ....|PPPP PPPP PPPP PPPP|NNNN NNNN NNNN NNNN|CCCC|....
{
	t_pheader	header;
	t_mrt_vec	position;
	t_mrt_vec	norm;
	t_mrt_color	color;
}	t_pobj_plane;

typedef struct s_parsed_cylinder// |HHHH HHHH HHHH ....|CCCC CCCC CCCC CCCC|NNNN NNNN NNNN NNNN|DDDD|HHHH|CCCC|....
{
	t_pheader	header;
	t_mrt_vec	center;
	t_mrt_vec	norm;
	t_f32		diameter;
	t_f32		height;
	t_mrt_color	color;
}	t_pobj_cylinder;

t_pheader	*mrt_pobj_new(t_objtype type);

void		mrt_pobj_clean(t_pheader *list);

void		mrt_pobj_push(t_pheader **head, t_pheader *pobj);

void		mrt_pobj_print_bin(t_pheader *head, t_u32 type);

// LINES ******************************************************************** //

# define MRT_LINE_LEN 248

typedef struct s_line
{
	struct s_line	*next;
	char			str[MRT_LINE_LEN];
}	t_line;

t_line		*mrt_line_new(const char *str, t_bool copy, t_u32 size);

void		mrt_line_clean(t_line *list);

void		mrt_line_copy(t_line *line, const char *str, t_u32 size);

void		mrt_line_push(t_line **list, t_line *new);

t_u32		mrt_line_count(t_line *list);

// PARSER ******************************************************************* //

typedef struct s_mrt_parser
{
	const char	*filename;
	t_s32		fd;
	t_u32		obj_count;
	t_pheader	*objs;
}	t_parser;

t_error		mrt_parse_file(const char *filename, t_parser *parser);

t_error		mrt_parse_int(int *res, char *str, char **remain);

t_error		mrt_parse_float(float *res, char *str, char **remain);

t_error		mrt_parse_vec(t_mrt_vec *res, char *str, char **remain);

t_error		mrt_parse_color(t_mrt_color *res, char *str, char **remain);

t_error		mrt_parse_pobj(t_pheader **obj, char *str, char **remain);

t_objtype	mrt_parse_pobj_type(char *str, char **remain);

t_error		mrt_parse_pobj_data(t_pheader **obj, char *fmt, char *str, char **remain);

#endif	// PARSER_H
