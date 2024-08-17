// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   parser.h                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/13 20:08:34 by rgramati          #+#    #+#             //
//   Updated: 2024/08/17 21:54:46 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PARSER_H
# define PARSER_H

# include <mrtlib.h>

typedef enum e_mrt_error	t_error;

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

struct	s_pobj_header
{
	t_pheader	*next;
	t_u32		obj_type;
};

typedef struct s_parsed_ambient
{
	t_pheader	header;
	t_f32		ratio;
	t_mrt_color	color;
}	t_pobj_ambient;

typedef struct s_parsed_camera
{
	t_pheader	header;
	t_mrt_vec	viewpoint;
	t_mrt_vec	orientation;
	t_u32		fov;
}	t_pobj_camera;

typedef struct s_parsed_light
{
	t_pheader	header;
	t_mrt_vec	lightpoint;
	t_f32		ratio;
}	t_pobj_light;

typedef struct s_parsed_sphere
{
	t_pheader	header;
	t_mrt_vec	center;
	t_f32		diameter;
	t_mrt_color	color;
}	t_pobj_sphere;

typedef struct s_parsed_plane
{
	t_pheader	header;
	t_mrt_vec	position;
	t_mrt_vec	norm;
	t_mrt_color	color;
}	t_pobj_plane;

typedef struct s_parsed_cylinder
{
	t_pheader	header;
	t_mrt_vec	center;
	t_mrt_vec	norm;
	t_f32		diameter;
	t_f32		height;
	t_mrt_color	color;
}	t_pobj_cylinder;

t_pheader	*mrt_pobj_new(t_pheader header);

void		mrt_pobj_clean(t_pheader *list);

// LINES ******************************************************************** //

# define MRT_LINE_LEN 248

typedef struct s_line
{
	struct s_line	*next;
	char			str[MRT_LINE_LEN];
}	t_line;

# define MRT_PRINT_LINES(X) t_line *tmp = (X); while (tmp) {printf("line = [%s]", tmp->str); tmp = tmp->next;}

t_line		*mrt_line_new(const char *str, t_bool copy, t_u32 size);

void		mrt_line_clean(t_line *list);

void		mrt_line_copy(t_line *line, const char *str, t_u32 size);

void		mrt_line_push(t_line **list, t_line *new);

t_u32		mrt_line_count(t_line *list);

// ************************************************************************** //

typedef struct s_mrt_parser
{
	const char	*filename;
	t_s32		fd;
	t_u32		obj_count;
	t_pheader	*objs;
}	t_parser;

t_error		mrt_parse_file(const char *filename, t_parser *parser);

t_error		mrt_parse_line_list(t_line **list, t_parser *parser);

t_error		mrt_parse_line_to_pobj(t_pheader **list, t_line *file);


t_objtype	mrt_parse_pobj_type(char *str, char **remain);

t_error		mrt_parse_pobj_light(t_pheader **obj, char *str, char **remain, int amb);

t_error		mrt_parse_pobj_camera(t_pheader **obj, char *str, char **remain);

t_error		mrt_parse_pobj_sphere(t_pheader **obj, char *str, char **remain);

t_error		mrt_parse_pobj_plane(t_pheader **obj, char *str, char **remain);

t_error		mrt_parse_pobj_cylinder(t_pheader **obj, char *str, char **remain);

#endif	// PARSER_H
