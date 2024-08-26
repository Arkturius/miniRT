// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_scene_render.c                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/26 18:42:44 by rgramati          #+#    #+#             //
//   Updated: 2024/08/26 22:02:26 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "mrt/parser.h"
#include <mlx.h>
#include <math.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>

#define MRT_CAMERA_FOV	0

t_f32	mrt_vec_dot(t_mrt_vec v1, t_mrt_vec v2)
{
	const t_f32 dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return (dot);
}

t_f32	mrt_vec_len(t_mrt_vec v)
{
	const t_f32	len = sqrtf(powf(v.x, 2.) + powf(v.y, 2.) + powf(v.z, 2.));

	return (len);
}

void	mrt_vec_norm(t_mrt_vec *v)
{
	const t_f32		len = mrt_vec_len(*v);
	const t_mrt_vec	ret = {.x = v->x / len, \
							.y = v->y / len, \
							.z = v->z / len};

	*v = ret;
}

void	mrt_vec_sub(t_mrt_vec *v1, t_mrt_vec v2)
{
	v1->x -= v2.x;
	v1->y -= v2.y;
	v1->z -= v2.z;
}

static void	mrt_ray_init(t_scene *scene, t_ray *ray, t_u32 x, t_u32 y)
{
	const t_objD	*cam = &scene->config[MRT_OBJ_CAMERA].data;
	const t_u32		*cam_data = (t_u32 *)cam->data;
	const double	ratio = tan(cam_data[MRT_CAMERA_FOV] * 0.5 * MRT_PI / 180);
	const double	trig[4] = {cos(cam->norm.x), cos(cam->norm.y), sin(cam->norm.x), sin(cam->norm.y)};
	t_mrt_vec		uv;

	uv.x = (ratio * ((2. * x - MRT_W) / MRT_H));
	uv.y = (ratio * ((2. * y - MRT_H) / MRT_H));
	uv.z = 1;
	mrt_vec_norm(&uv);
	ray->direction = (t_mrt_vec){
		.x = uv.x * trig[1] + uv.y * trig[2] * trig[3] - uv.z * trig[0] * trig[3],
		.y = uv.y * trig[0] + uv.z * trig[2],
		.z = uv.x * trig[3] - uv.y * trig[2] * trig[1] + uv.z * trig[0] * trig[1]};
	ray->origin = cam->pos;
}

# define MRT_SPHERE_DIAMETER	0

# define PARAM_A		0
# define PARAM_B		1
# define PARAM_C		2
# define PARAM_DELTA	3
# define PARAM_DELTA_R	4
# define PARAM_2_A		5

t_f32	mrt_fabs(t_f32 f)
{
	if (f < 0)
		return (-f);
	return (f);
}

void	mrt_sphere_inter(t_ray *ray, t_objD *data)
{
	t_mrt_vec	c_to_r;
	double		params[6] = {0};

	c_to_r = ray->origin;
	mrt_vec_sub(&c_to_r, data->pos);
	params[PARAM_A] = mrt_vec_dot(ray->direction, ray->direction);
	params[PARAM_B] = 2. * mrt_vec_dot(c_to_r, ray->direction);
	params[PARAM_C] = mrt_vec_dot(c_to_r, c_to_r) - powf(data->data[MRT_SPHERE_DIAMETER] / 2., 2);
	params[PARAM_DELTA] = powf(params[PARAM_B], 2) - (4 * params[PARAM_A] * params[PARAM_C]);
	if (params[PARAM_DELTA] < 0 || mrt_fabs(params[PARAM_B]) < 1e-3)
		return ;
	params[PARAM_DELTA_R] = sqrtf(params[PARAM_DELTA]);
	params[PARAM_2_A] = 2. * params[PARAM_A];
	if (mrt_fabs(params[PARAM_DELTA] < 1e-3))
	{
		ray->hit.dist = -params[PARAM_C] / params[PARAM_2_A];
		ray->hit.hit = MRT_TRUE;
		return ;
	}
	ray->hit.dist = ((-params[PARAM_A] - params[PARAM_DELTA_R]) / params[PARAM_2_A]);
	if (ray->hit.dist < 0)
		ray->hit.dist = ((-params[PARAM_A] + params[PARAM_DELTA_R]) / params[PARAM_2_A]);
	if (ray->hit.dist < 0)
		return ;
	ray->hit.hit = MRT_TRUE;
	ray->hit.color = data->mat.obj;
}

t_mrt_color	mrt_scene_get_color(t_scene *scene, t_u32 x, t_u32 y)
{
	t_ray	ray;

	mrt_bzero(&ray, sizeof(t_ray));
	mrt_ray_init(scene, &ray, x, y);
	ray.hit.dist = INFINITY;

	t_u32	i = 0;
	while (i < scene->nobj)
	{
		t_object	*obj = &scene->objects->objs[i];
		if (obj->type == MRT_OBJ_SPHERE)
			mrt_sphere_inter(&ray, &obj->data);
		i++;
	}
	if (ray.hit.hit)
		return (ray.hit.color);
	return ((t_mrt_color){.argb = -1});
}

int	mrt_scene_render(void *scene_ptr)
{
	t_u32		x;
	t_u32		y;
	t_scene		*scene;
	t_mrt_color	color;

	color = (t_mrt_color){0};
	scene = (t_scene *)scene_ptr;
	x = 0;
	while (x < MRT_W)
	{
		y = 0;
		while (y < MRT_H)
		{
			color = mrt_scene_get_color(scene, x, y);
			mlx_pixel_put(scene->mlx.app, scene->mlx.win, x, y, color.argb);
			y++;
		}
		x++;
	}
	return (MRT_SUCCESS);
}
