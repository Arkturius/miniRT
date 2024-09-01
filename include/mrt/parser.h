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

# define MRT_FORMAT				"ifcv"

# define MRT_FORMAT_AMBIENT		"56 !f 48 !c"

# define MRT_FORMAT_CAMERA		"16 !v 32 !v 8 !i"

# define MRT_FORMAT_LIGHT		"16 !v 56 !f 48 !c"

# define MRT_FORMAT_SPHERE		"16 !v 8 !f 48 !c"

# define MRT_FORMAT_PLANE		"16 !v 32 !v 48 !c"

# define MRT_FORMAT_CYLINDER	"16 !v 32 !v 8 !f 12 !f 48 !c"

// typedef struct s_mrt_object
// {
// 	t_u32		type;
// 	t_u32		id;
// 	t_f32		data[2];
// 	t_mrt_vec	pos;
// 	t_mrt_vec	norm;
// 	t_material	mat;
// }	t_object;

typedef struct s_mrt_object	t_object;

t_error																		\
mrt_parse_obj(t_object *ptr, char *str, char **remain);

	/*	Formatted stream function.
 *	128 bytes to write [] = 4 bytes
 *	
 *	[type]			[id]		[diameter/fov]	[height]
 *	[pos.x]			[pos.y]		[pos.z]			[pos.w]
 *	[norm.x]		[norm.y]	[norm.z]		[norm.w]	norm for cam is orientation
 *	[c|obj]			[c|emi]		[f|obj_r]		[f|emi_r]
 *	
 *	so for:
 *		- sphere:	"!c +c +f +f !v +v !f", obj.color, obj.center, obj.diameter
 *		- plane:	"!c +c +f +f !v !v", obj.color, obj.position, obj.norm
 *		- cylinder:	"!c +c +f +f !v !v !f !f", obj.color, obj.center, obj.norm, obj.diameter, obj.height
 *
 */

#endif	// PARSER_H
