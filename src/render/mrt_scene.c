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

static t_error	mrt_parse_rt(t_scene *scene, char *stream)
{
	t_error	err;

	err = (t_error){MRT_SUCCESS, (void *)__func__};
	while (*stream && err.type == MRT_SUCCESS)
	{
		err = mrt_parse_obj(scene, stream, &stream);
		if (err.type != MRT_SUCCESS)
			return (err);
		while (*stream && mrt_isspace((unsigned char)*stream))
			++stream;
		scene->nobj++;
	}
	return (err);
}

#ifndef MRT_BONUS

static t_error	mrt_scene_objs_init(t_scene *scene, t_file *file)
{
	t_error	err;

	err = (t_error){MRT_SUCCESS, (void *)__func__};
	scene->objects = mrt_obj_chunk_init();
	scene->nobj = 0;
	if (scene->objects)
		err = mrt_parse_rt(scene, file->data);
	else
		err.type = MRT_ERR_ALLOC;
	return (err);
}

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


// TODO: MOVE PARSE FUNCTIONS AWAY
#else

static t_error	mrt_parse_cub(t_scene *scene, char *stream)
{
	(void) scene;
	(void) stream;
	return ((t_error){MRT_SUCCESS, NULL});
}

static t_error	mrt_scene_objs_init(t_scene *scene, t_file *file)
{
	t_error	err;

	err = (t_error){MRT_SUCCESS, (void *)__func__};
	scene->objects = mrt_obj_chunk_init();
	scene->lights = mrt_obj_chunk_init();
	scene->nobj = 0;
	if (scene->objects)
	{
		if (file->type == MRT_FILE_CUB)
			err = mrt_parse_cub(scene, file->data);
		else if (file->type == MRT_FILE_RT)
			err = mrt_parse_rt(scene, file->data);
	}
	else
		err.type = MRT_ERR_ALLOC;
	return (err);
}

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

#ifndef MRT_BONUS

t_error	mrt_scene_init(t_file *file, t_scene *scene)
{
	t_error	err;

	mrt_bzero(scene, sizeof(t_scene));
	err = mrt_scene_objs_init(scene, file);
	if (err.type == MRT_SUCCESS)
		err = mrt_scene_obj_config(scene);
	if (err.type == MRT_SUCCESS)
		mrt_scene_cam_init(scene);
	scene->config[0].data[1] = 0;
	return (err);
}

#else

void mrt_triangle_normal(t_mrt_vec *normal, t_mrt_vec *a, t_mrt_vec *b, t_mrt_vec *c)
{
    t_mrt_vec edge1, edge2;

    mrt_vec_sub(*b, *a, &edge1); // edge1 = B - A
    mrt_vec_sub(*c, *a, &edge2); // edge2 = C - A

    mrt_vec_cross(edge1, edge2, normal);

    mrt_vec_norm(*normal, normal);
}

#include <stdlib.h>
t_error	mrt_scene_init(t_file *file, t_scene *scene)
{
	t_error	err;

	mrt_bzero(scene, sizeof(t_scene));
	err = mrt_scene_objs_init(scene, file);
	if (err.type == MRT_SUCCESS)
		err = mrt_scene_obj_config(scene);
	if (err.type == MRT_SUCCESS)
		mrt_scene_cam_init(scene);

	scene->points = malloc(sizeof(t_pbuffer));				//INIT BUFFERS
	mrt_bzero(scene->points, sizeof(t_pbuffer));
	scene->triangles = malloc(sizeof(t_tbuffer));
	mrt_bzero(scene->triangles, sizeof(t_tbuffer));

	t_object	*obj = mrt_obj_alloc(scene->objects, MRT_TRUE);	//FIRST TRIANGLE
	mrt_bzero(obj, sizeof(t_object));
	obj->type = MRT_OBJ_OBJFILE;
	obj->data[0] = 0;
	obj->data[1] = (float)1;
	obj->mat.obj = (t_mrt_color){.argb = 0xFFFFFF00};

	scene->triangles->data[0] = (t_triangle){{0, 1, 2, 0}};
	scene->points->data[0] = (t_mrt_vec){0, 0, 0, 0};
	scene->points->data[1] = (t_mrt_vec){0, 2, 0, 0};
	scene->points->data[2] = (t_mrt_vec){0, 2, 2, 0};

	mrt_triangle_normal(&obj->norm, &scene->points->data[0], &scene->points->data[1], &scene->points->data[2]);
	obj->norm = (t_mrt_vec){.x = -obj->norm.x, .y = -obj->norm.y, .z = -obj->norm.z};

	obj = mrt_obj_alloc(scene->objects, MRT_TRUE);				//SECOND TRIANGLE
	mrt_bzero(obj, sizeof(t_object));
	obj->type = MRT_OBJ_OBJFILE;
	obj->data[0] = 1;
	obj->data[1] = (float)2;
	obj->mat.obj = (t_mrt_color){.argb = 0xFF00FF00};

	scene->triangles->data[1] = (t_triangle){{0, 2, 3, 0}};
	scene->points->data[3] = (t_mrt_vec){0, 0, 2, 0};

	mrt_triangle_normal(&obj->norm, &scene->points->data[1], &scene->points->data[2], &scene->points->data[3]);
	obj->norm = (t_mrt_vec){.x = -obj->norm.x, .y = -obj->norm.y, .z = -obj->norm.z};
	
	scene->triangles->size = 2;
	scene->points->size = 4;

	scene->config[0].data[1] = 1;
	return (err);
}

#endif
