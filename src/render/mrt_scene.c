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

#include <math.h>

#include <mrtlib.h>

#include <mrt/error.h>
#include <mrt/parser.h>
#include <mrt/engine.h>

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
			scene->nobj++;
		}
	}
	else
		err.type = MRT_ERR_ALLOC;
	return (err);
}

#ifndef MRT_BONUS

static t_error	mrt_scene_obj_config(t_scene *scene)
{
	t_error		err;
	uint32_t	count;

	count = 0;
	err = (t_error){MRT_SUCCESS, (void *)__func__};
	count += !!scene->config[MRT_OBJ_CAMERA].type;
	count += !!scene->config[MRT_OBJ_AMBIENT].type;
	count += !!scene->config[MRT_OBJ_LIGHT].type;
	if (count != 3)
		err.type = MRT_ERR_FMT_CONFIG;
	return (err);
}

#else

static t_error	mrt_scene_obj_config(t_scene *scene)
{
	t_error		err;
	uint32_t	count;

	count = 0;
	err = (t_error){MRT_SUCCESS, (void *)__func__};
	count += !!scene->config[MRT_OBJ_CAMERA].type;
	count += !!scene->config[MRT_OBJ_AMBIENT].type;
	if (count != 2)
		err.type = MRT_ERR_FMT_CONFIG;
	return (err);
}

#endif

void	mrt_scene_cam_init(t_scene *scene)
{
	const t_mrt_vec	look = scene->config[MRT_OBJ_CAMERA].norm;
	const double	alpha = -acos(look.z / sqrt(pow(look.x, 2.) \
							+ pow(look.z, 2.)));
	const double	beta = atan2(look.y, look.x);

	scene->config[MRT_OBJ_CAMERA].norm = \
		(t_mrt_vec){.x = beta, .y = alpha, .z = 0, .w = 0};
}

t_error	mrt_scene_init(t_file *file, t_scene *scene)
{
	t_error	err;

	mrt_bzero(scene, sizeof(t_scene));
	err = mrt_scene_obj_init(scene, file);
	if (err.type == MRT_SUCCESS)
		err = mrt_scene_obj_config(scene);
	if (err.type == MRT_SUCCESS)
		mrt_scene_cam_init(scene);
	return (err);
}
