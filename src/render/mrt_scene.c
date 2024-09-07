/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:06:49 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/01 23:55:16 by rems             ###   ########.fr       */
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
	t_error	err;
	char	*stream;

	err = (t_error){MRT_SUCCESS, (void *)__func__};
	scene->objects = mrt_obj_chunk_init();
	scene->nobj = 0;
	if (scene->objects)
	{
		stream = file->data;
		while (*stream && err.type == MRT_SUCCESS)
		{
			err = mrt_parse_obj(scene, stream, &stream);
			if (err.type != MRT_SUCCESS)
				return (err);
			while (*stream && mrt_isspace((unsigned char)*stream))
				++stream;
		}
		scene->nobj = mrt_scene_obj_count(scene->objects);
	}
	else
		err.type = MRT_ERR_ALLOC;
	return (err);
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
	t_error	err;

	err = mrt_scene_obj_init(scene, file);
	if (err.type == MRT_SUCCESS)
	{
		scene->map = malloc(65536 * sizeof(char));
		if (!scene->map)
		{
			err.type = MRT_ERR_ALLOC;
			return (err);
		}
		mrt_bzero(scene->map, 65536);
		mrt_scene_cam_init(scene);
	}
	return (err);
}
