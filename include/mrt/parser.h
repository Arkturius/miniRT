/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:08:34 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/25 19:23:25 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <mrtlib.h>
# include <mrt/error.h>

/* PARSED OBJS HOLDERS ****************************************************** */

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

# define MRT_PFORMAT			"ifvc"
struct	s_pobj_header
{
	t_pheader	*next;
	t_u32		obj_type;
};

# define MRT_PFORMAT_AMBIENT	"!f !c"
typedef struct s_parsed_ambient
{
	t_pheader	header;
	t_f32		ratio;
	t_mrt_color	color;
}	t_pobj_ambient;

# define MRT_PFORMAT_CAMERA		"!v !v !i"
typedef struct s_parsed_camera
{
	t_pheader	header;
	t_mrt_vec	viewpoint;
	t_mrt_vec	orientation;
	t_s32		fov;
}	t_pobj_camera;

# define MRT_PFORMAT_LIGHT		"!v !f !c"
typedef struct s_parsed_light
{
	t_pheader	header;
	t_mrt_vec	lightpoint;
	t_f32		ratio;
	t_mrt_color	color;
}	t_pobj_light;

# define MRT_PFORMAT_SPHERE		"!v !f !c"
typedef struct s_parsed_sphere
{
	t_pheader	header;
	t_mrt_vec	center;
	t_f32		diameter;
	t_mrt_color	color;
}	t_pobj_sphere;

# define MRT_PFORMAT_PLANE		"!v !v !c"
typedef struct s_parsed_plane
{
	t_pheader	header;
	t_mrt_vec	position;
	t_mrt_vec	norm;
	t_mrt_color	color;
}	t_pobj_plane;

# define MRT_PFORMAT_CYLINDER	"!v !v !f !f !c"
typedef struct s_parsed_cylinder
{
	t_pheader	header;
	t_mrt_vec	center;
	t_mrt_vec	norm;
	t_f32		diameter;
	t_f32		height;
	t_mrt_color	color;
}	t_pobj_cylinder;

t_pheader																	\
*mrt_pobj_new(t_objtype type);

void																		\
mrt_pobj_clean(t_pheader *list);

void																		\
mrt_pobj_push(t_pheader **head, t_pheader *pobj);

t_pheader																	\
*mrt_pobj_pop(t_pheader **head);

/* LINES ******************************************************************** */

# define MRT_LINE_LEN 248
typedef struct s_line
{
	struct s_line	*next;
	char			str[MRT_LINE_LEN];
}	t_line;

t_line																		\
*mrt_line_new(const char *str, t_bool copy, t_u32 size);

void																		\
mrt_line_clean(t_line *list);

void																		\
mrt_line_copy(t_line *line, const char *str, t_u32 size);

void																		\
mrt_line_push(t_line **list, t_line *new);

t_u32																		\
mrt_line_count(t_line *list);

/* PARSER ******************************************************************* */

typedef struct s_mrt_file
{
	const char	*filename;
	t_s32		fd;
	t_u32		obj_count;
	t_pheader	*objs;
	char		*data;
}	t_file;

t_error																		\
mrt_parse_file(const char *name, t_file *file);

t_error																		\
mrt_parse_pobj_all(t_file *file);

t_error																		\
mrt_parse_int(int *res, char *str, char **remain);

t_error																		\
mrt_parse_float(float *res, char *str, char **remain);

t_error																		\
mrt_parse_vec(t_mrt_vec *res, char *str, char **remain);

t_error																		\
mrt_parse_color(t_mrt_color *res, char *str, char **remain);

# define MRT_FORMAT_AMBIENT		"!c +c !f"
# define MRT_FORMAT_CAMERA		"+c +c +f +f !v !v !i"
# define MRT_FORMAT_LIGHT		"!c +c !f +f !v"
# define MRT_FORMAT_SPHERE		"!c +c +f +f !v +v !f"
# define MRT_FORMAT_PLANE		"!c +c +f +f !v !v"
# define MRT_FORMAT_CYLINDER	"!c +c +f +f !v !v !f !f"

/*	Formatted stream function.
 *	128 bytes to write [] = 4 bytes
 *	
 *	[c|obj]			[c|emi]		[f|obj_r]	[f|emi_r]
 *	[pos.x]			[pos.y]		[pos.z]		[pos.w]
 *	[norm.x]		[norm.y]	[norm.z]	[norm.w]	norm for cam is orientation
 *	[diameter/fov]	[height]	[NULL]		[NULL]
 *	
 *	so for:
 *		- sphere:	"!c +c +f +f !v +v !f", obj.color, obj.center, obj.diameter
 *		- plane:	"!c +c +f +f !v !v", obj.color, obj.position, obj.norm
 *		- cylinder:	"!c +c +f +f !v !v !f !f", obj.color, obj.center, obj.norm, obj.diameter, obj.height
 *
 */

#endif	// PARSER_H
