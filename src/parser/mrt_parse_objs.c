/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_parse_objs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 18:41:03 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/25 19:39:26 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>
#include <mrt/engine.h>

static void mrt_parse_obj_step(t_u8 **ptr, char *fmt)
{
	t_u32	off;

	off = 0;
	if (*fmt == 'v')
		off = sizeof(t_mrt_vec);
	else
		off = sizeof(int);
	*ptr += off;
}

static void	mrt_parse_obj_value(t_u8 **ptr, va_list ap, char *fmt)
{
	t_u32		off;

	if (*fmt == 'v')
	{
		*(t_mrt_vec *)*ptr = va_arg(ap, t_mrt_vec);
		off = sizeof(t_mrt_vec);
	}
	if (*fmt == 'f')
	{
		*(t_f32 *)*ptr = (t_f32) va_arg(ap, double);
		off = sizeof(t_f32);
	}
	if (*fmt == 'i')
	{
		*(t_s32 *)*ptr = (t_s32) va_arg(ap, t_s32);
		off = sizeof(t_s32);
	}
	if (*fmt == 'c')
	{
		*(t_u32 *)*ptr = (t_u32) va_arg(ap, t_u32);
		off = sizeof(t_u32);
	}
	*ptr += off;
}

static t_error	mrt_parse_obj_format(t_objD *data, char *fmt, ...)
{
	va_list	ap;
	t_u8	*ptr;

	ptr = (t_u8 *)data;
	if (!ptr)
		return (MRT_FAIL);
	va_start(ap, fmt);
	while (*fmt && *(fmt + 1))
	{
		if (mrt_strchr(MRT_PFORMAT, *(fmt + 1)))
		{
			if (*fmt == '!')
				mrt_parse_obj_value(&ptr, ap, ++fmt);
			else if (*fmt == '+')
				mrt_parse_obj_step(&ptr, ++fmt);
		}
		fmt++;
	}
	va_end(ap);
	return (MRT_SUCCESS);
}

t_error	mrt_parse_obj_regular(t_object *object, t_pheader *header)
{
	t_pobj_sphere	*sp;
	t_pobj_plane	*pl;
	t_pobj_cylinder	*cy;
	t_error			err;

	err = MRT_SUCCESS;
	if (object->type == MRT_OBJ_SPHERE)
	{
		sp = (t_pobj_sphere *)header;
		err = mrt_parse_obj_format(&object->data, MRT_FORMAT_SPHERE, \
			sp->color, sp->center, sp->diameter);
	}
	if (object->type == MRT_OBJ_PLANE)
	{
		pl = (t_pobj_plane *)header;
		err = mrt_parse_obj_format(&object->data, MRT_FORMAT_PLANE, \
			pl->color, pl->position, pl->norm);
	}
	if (object->type == MRT_OBJ_CYLINDER)
	{
		cy = (t_pobj_cylinder *)header;
		err = mrt_parse_obj_format(&object->data, MRT_FORMAT_CYLINDER, \
			cy->color, cy->center, cy->norm, cy->diameter, cy->height);	
	}
	return (err);
}

t_error	mrt_parse_obj_config(t_object *object, t_pheader *header)
{
	t_pobj_ambient	*a;
	t_pobj_camera	*c;
	t_pobj_light	*l;
	t_error			err;

	err = MRT_SUCCESS;
	if (object->type == MRT_OBJ_AMBIENT)
	{
		a = (t_pobj_ambient *)header;
		err = mrt_parse_obj_format(&object->data, MRT_FORMAT_AMBIENT, \
			a->color, a->ratio);
	}
	if (object->type == MRT_OBJ_CAMERA)
	{
		c = (t_pobj_camera *)header;
		err = mrt_parse_obj_format(&object->data, MRT_FORMAT_CAMERA, \
			c->viewpoint, c->orientation, c->fov);
	}
	if (object->type == MRT_OBJ_LIGHT)
	{
		l = (t_pobj_light *)header;
		err = mrt_parse_obj_format(&object->data, MRT_FORMAT_LIGHT, \
			l->color, l->ratio, l->lightpoint);
	}
	return (err);
}

// ACTUALLY i want to merge this witrh pobjs parsing cause 2 formatted string functions when i can have only ONE

// noice this is working. TODO implement real conversion from pobjs to objects
// TODO implement meshes and triangle buffers
// TODO bvh magic
