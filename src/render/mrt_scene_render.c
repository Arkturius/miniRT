/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_scene_render.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:42:44 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/28 15:38:13 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include <mlx.h>
#include <SDL2/SDL_scancode.h>

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
	const t_object	*cam = &scene->config[MRT_OBJ_CAMERA];
	const t_u32		*fov_addr = (t_u32 *)&cam->data[MRT_CAMERA_FOV];
	const double	ratio = tan(*fov_addr * 0.5 * MRT_PI / 180);
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

void	mrt_sphere_inter(t_ray *ray, t_object *obj)
{
	t_mrt_vec	c_to_r;
	double		params[6] = {0};
	t_f32		dist;

	c_to_r = ray->origin;
	mrt_vec_sub(&c_to_r, obj->pos);
	params[PARAM_A] = mrt_vec_dot(ray->direction, ray->direction);
	params[PARAM_B] = 2. * mrt_vec_dot(c_to_r, ray->direction);
	params[PARAM_C] = mrt_vec_dot(c_to_r, c_to_r) - powf(obj->data[MRT_OBJ_SPHERE_DIAMETER] / 2., 2);
	params[PARAM_DELTA] = powf(params[PARAM_B], 2) - (4 * params[PARAM_A] * params[PARAM_C]);
	if (params[PARAM_DELTA] < 0 || mrt_fabs(params[PARAM_B]) < 1e-3)
		return ;
	params[PARAM_DELTA_R] = sqrtf(params[PARAM_DELTA]);
	params[PARAM_2_A] = 2. * params[PARAM_A];
	if (mrt_fabs(params[PARAM_DELTA] < 1e-3))
		dist = -params[PARAM_B] / params[PARAM_2_A];
	else
	{
		dist = ((-params[PARAM_B] - params[PARAM_DELTA_R]) / params[PARAM_2_A]);
		if (dist < 0)
			dist = ((-params[PARAM_B] + params[PARAM_DELTA_R]) / params[PARAM_2_A]);
		if (dist < 0)
			return ;
	}
	if (dist > 0 && dist < ray->hit.dist)
	{
		ray->hit.color = obj->mat.obj;
		ray->hit.hit = MRT_TRUE;
		ray->hit.dist = dist;
	}
}

t_mrt_color	mrt_scene_get_color(t_scene *scene, t_u32 x, t_u32 y)
{
	t_ray		ray;
	t_object	*obj;

	mrt_bzero(&ray, sizeof(t_ray));
	mrt_ray_init(scene, &ray, x, y);
	ray.hit.dist = INFINITY;

	t_u32	i = 0;
	while (i < scene->nobj)
	{
		obj = &scene->objects->objs[i];
		if (obj->type == MRT_OBJ_SPHERE)
			mrt_sphere_inter(&ray, obj);
		i++;
	}
	if (ray.hit.hit)
		return (ray.hit.color);
	return (scene->config[MRT_OBJ_AMBIENT].mat.obj);
}

static void	mrt_scene_transform(t_scene *scene)
{
	t_f32		speed;
	t_mrt_vec	campos;
	t_mrt_vec	angles;

	speed = 0.2;
	campos = scene->config[MRT_OBJ_CAMERA].pos;
	angles = scene->config[MRT_OBJ_CAMERA].norm;
	if (scene->map[SDL_SCANCODE_LSHIFT])
		campos.y -= speed;
	if (scene->map[SDL_SCANCODE_SPACE])
		campos.y += speed;
	if (scene->map[SDL_SCANCODE_W])
		campos.x += speed;
	if (scene->map[SDL_SCANCODE_A])
		campos.z -= speed;
	if (scene->map[SDL_SCANCODE_S])
		campos.x -= speed;
	if (scene->map[SDL_SCANCODE_D])
		campos.z += speed;
	if (scene->map[SDL_SCANCODE_Q])
		angles.y += 0.02;
	if (scene->map[SDL_SCANCODE_E])
		angles.y -= 0.02;
	if (scene->map[SDL_SCANCODE_P])
		printf("Camera {\n  pos: %f %f %f\n  dir: %f %f %f\n}\n", campos.x, campos.y, campos.z, angles.x, angles.y, angles.z);
	scene->config[MRT_OBJ_CAMERA].pos = campos;
	scene->config[MRT_OBJ_CAMERA].norm = angles;
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
			color = mrt_scene_get_color(scene, x, MRT_H - y - 1);
			mlx_pixel_put(scene->mlx.app, scene->mlx.win, x, y, color.argb);
			y++;
		}
		x++;
	}
	mrt_scene_transform(scene);
	return (MRT_SUCCESS);
}
