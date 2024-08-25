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

t_error	mrt_parse_obj_format(t_objD *data, char *fmt, ...)
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

// noice this is working. TODO implement real conversion from pobjs to objects
// TODO implement meshes and triangle buffers
// TODO bvh magic

// #include <stdio.h>
// #include <stdlib.h>
//
// int main(void)
// {
// 	t_objD	*test = malloc(sizeof(t_objD));
//
// 	mrt_parse_obj_format(test, "!c +c +f +f !v +v !f", (t_mrt_color){.argb = (t_u32)-1}, (t_mrt_vec){.x = 1, .y = 2, .z = 6.2364, .w = 0}, 0.3);
//
// 	printf("%3u %3f %3u %3f ;\n %3f %3f %3f %3f ;\n %3f %3f %3f %3f ;\n %3f %3f %3f %3f\n", test->mat.obj.argb, test->mat.obj_r, test->mat.emi.argb, test->mat.emi_r, test->pos.x, test->pos.y, test->pos.z, test->pos.w, test->norm.x, test->norm.y, test->norm.z, test->norm.w, test->data[0], test->data[1], test->data[2], test->data[3]);
// }


t_error	mrt_parse_obj_data(t_objD *data, t_pheader *header)
{
	if (header->obj_type == MRT_OBJ_AMBIENT)
	{
		data->mat.obj = ((t_pobj_ambient *) header)->color;
		data->mat.obj_r = ((t_pobj_ambient *) header)->ratio;
	}
	if (header->obj_type != MRT_OBJ_AMBIENT && \
		header->obj_type != MRT_OBJ_LIGHT && \
		header->obj_type != MRT_OBJ_AMBIENT)
		data->pos = *(t_mrt_vec *)((t_u8 *)header + sizeof(t_pheader));
	else
		data->pos = (t_mrt_vec){0};
	if (header->obj_type == MRT_OBJ_PLANE || header->obj_type == MRT_OBJ_CYLINDER)
		data->norm = *(t_mrt_vec *)((t_u8 *)header + sizeof(t_pheader) + sizeof(t_mrt_vec));
	else
		data->norm = (t_mrt_vec){0};
	if (header->obj_type == MRT_OBJ_SPHERE)
		data->data[0] = *(t_f32 *)((t_u8 *)header + sizeof(t_pheader) + sizeof(t_mrt_vec));
	if (header->obj_type == MRT_OBJ_CYLINDER)
		data->data[0] = *(t_f32 *)((t_u8 *)header + sizeof(t_pheader) + 2 * sizeof(t_mrt_vec));
	if (header->obj_type == MRT_OBJ_CYLINDER)
		data->data[1] = *(t_f32 *)((t_u8 *)header + sizeof(t_pheader) + 2 * sizeof(t_mrt_vec) + sizeof(t_f32));
	return (MRT_SUCCESS);
}

t_error	mrt_parse_obj_color(t_objD *data, t_pheader *header)
{
	data->mat.obj_r = 1;
	if (header->obj_type == MRT_OBJ_SPHERE)
		data->mat.obj = ((t_pobj_sphere *)header)->color;
	if (header->obj_type == MRT_OBJ_PLANE)
		data->mat.obj = ((t_pobj_plane *)header)->color;
	if (header->obj_type == MRT_OBJ_CYLINDER)
		data->mat.obj = ((t_pobj_cylinder *)header)->color;
	return (MRT_SUCCESS);
}

t_error	mrt_parse_obj(t_object *object, t_pheader *header)
{
	static int	id = 0;

	object->type = header->obj_type;
	object->id = id++;
	mrt_parse_obj_data(&object->data, header);
	mrt_parse_obj_color(&object->data, header);
	return (MRT_SUCCESS);
}

t_error mrt_parse_objs_all(t_scene *scene, t_file *file)
{
	(void) scene;
	(void) file;
	t_pheader	*header;
	t_object	*object;
	// t_u32		count;

	// count = 0;
	scene->objects = mrt_obj_chunk_init();
	scene->nobj = 0;
	if (!scene->objects)
		return (MRT_ERR_ALLOC);
	while (file->objs)
	{
		header = mrt_pobj_pop(&file->objs);
		object = mrt_obj_alloc(scene->objects, MRT_TRUE);
		mrt_parse_obj(object, header);
		scene->nobj++;
		// count++;
	}
	// mrt_io_save("objects.mrt", "@> ", scene->objects, count + 1, sizeof(t_object));
	return (MRT_FAIL);
}
