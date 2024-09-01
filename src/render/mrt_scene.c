/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:06:49 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/28 15:14:02 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>

#include <mrtlib.h>

#include <mrt/error.h>
#include <mrt/parser.h>
#include <mrt/engine.h>

static t_u32	mrt_scene_obj_count(t_obj_chunk *chunk)
{
	t_u32	count;

	count = 0;
	while (chunk)
	{
		count += chunk->size;
		chunk = chunk->next;
	}
	return (count);
}

static t_error	mrt_scene_obj_init(t_scene *scene, t_file *file)
{
	t_object	*obj;
	char		*stream;

	scene->objects = mrt_obj_chunk_init();
	scene->nobj = 0;
	if (!scene->objects)
		return (MRT_ERR_ALLOC);
	stream = file->data;
	while (*stream)
	{
		obj = mrt_obj_alloc(scene->objects, MRT_TRUE);
		if (mrt_parse_obj(obj, stream, &stream))
			return (MRT_FAIL);
		if (obj->type < MRT_OBJ_SPHERE)
			mrt_memcpy(&scene->config[obj->type], obj, sizeof(t_object));
		if (obj->type < MRT_OBJ_SPHERE)
			mrt_obj_free(scene->objects);	
		while (*stream && mrt_isspace((unsigned char)*stream))
			++stream;
	}
	scene->nobj = mrt_scene_obj_count(scene->objects);
	return (MRT_SUCCESS);
}

static void	mrt_scene_cam_init(t_scene *scene)
{
	const t_mrt_vec	look = scene->config[MRT_OBJ_CAMERA].norm;
	const double	alpha = -acos(look.z / sqrt(pow(look.x, 2.) \
							+ pow(look.z, 2.)));
	const double	beta = atan2(look.y, look.x);

	scene->config[MRT_OBJ_CAMERA].norm = \
		(t_mrt_vec){.x = alpha, .y = beta, .z = 0, .w = 0};
}

void	mrt_scene_clean(t_scene *scene)
{
	mrt_obj_chunk_clean(scene->objects);
	mrt_mlx_clean(scene->mlx);
	(void) scene;
}

t_error	mrt_scene_init(t_file *file, t_scene *scene)
{
	if (mrt_scene_obj_init(scene, file))
		return (MRT_FAIL);
	scene->map = malloc(65536 * sizeof(char));
	if (!scene->map)
		return (mrt_error_alloc(__func__));
	mrt_bzero(scene->map, 65536);
	mrt_scene_cam_init(scene);
	// mrt_scene_aabb_init(scene);
	// mrt_io_save("save/objects.mrt", "@> ", scene->objects, 32, sizeof(t_object));
	// mrt_io_save("save/camera.mrt", "@ ", &scene->config[MRT_OBJ_CAMERA].data, 1, sizeof(t_objD));
	return (MRT_SUCCESS);
}
