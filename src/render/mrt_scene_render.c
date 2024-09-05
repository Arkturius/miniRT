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

#include "mrt/parser.h"
#include <math.h>

#include <mlx.h>
#include <SDL2/SDL_scancode.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>

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

t_mrt_vec	mrt_vec_cross(t_mrt_vec v1, t_mrt_vec v2)
{
	return ((t_mrt_vec){.x = v1.y * v2.z - v2.y * v1.z,
						.y = v1.z * v2.x - v1.x * v2.z,
						.z = v1.x * v2.y - v2.x * v1.y,
						.w = 0});
}

void	mrt_vec_sub(t_mrt_vec *v1, t_mrt_vec v2)
{
	v1->x -= v2.x;
	v1->y -= v2.y;
	v1->z -= v2.z;
}

void	mrt_vec_add(t_mrt_vec *v1, t_mrt_vec v2)
{
	v1->x += v2.x;
	v1->y += v2.y;
	v1->z += v2.z;
}

void	mrt_vec_mult(t_mrt_vec *v1, t_f32 f)
{
	v1->x *= f;
	v1->y *= f;
	v1->z *= f;
}

static void	mrt_ray_init(t_scene *scene, t_ray *ray, t_u32 x, t_u32 y)
{
	const t_object	*cam = &scene->config[MRT_OBJ_CAMERA];
	const t_u32		*fov_addr = (t_u32 *)&cam->data[MRT_OBJ_CAMERA_FOV];
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
		.z = uv.x * trig[3] - uv.y * trig[2] * trig[1] + uv.z * trig[0] * trig[1],
		.w = 1};
	ray->origin = cam->pos;
	ray->hit.dist = INFINITY;
	ray->hit.color = (t_mrt_color){.a = 255};
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

t_u32	mrt_min(t_u32 a, t_u32 b)
{
	if (a < b)
		return (a);
	return (b);
}

t_u32	mrt_max(t_u32 a, t_u32 b)
{
	if (a >= b)
		return (a);
	return (b);
}

t_u32	mrt_clamp(t_u32 f, t_u32 cmin, t_u32 cmax)
{
	return (mrt_min(mrt_max(f, cmin), cmax));
}

void	mrt_solve_poly(t_f32 *params, t_f32 *res)
{
	t_f32	dist;

	dist = 0;
	params[PARAM_DELTA] = powf(params[PARAM_B], 2) - \
		(4 * params[PARAM_A] * params[PARAM_C]);
	if (params[PARAM_DELTA] < 0 || mrt_fabs(params[PARAM_B]) < MRT_EPS)
		return ;
	params[PARAM_DELTA_R] = sqrtf(params[PARAM_DELTA]);
	params[PARAM_2_A] = 2. * params[PARAM_A];
	if (mrt_fabs(params[PARAM_DELTA] < MRT_EPS))
		dist = -params[PARAM_B] / params[PARAM_2_A];
	else
	{
		dist = ((-params[PARAM_B] - \
			params[PARAM_DELTA_R]) / params[PARAM_2_A]);
		if (dist < 0)
			dist = ((-params[PARAM_B] + \
				params[PARAM_DELTA_R]) / params[PARAM_2_A]);
		if (dist < 0)
			return ;
	}
	*res = dist;
}

static void	mrt_ray_update(t_ray *ray, t_object *obj, t_f32 dist)
{
	t_mrt_vec	path;

	path = ray->direction;
	mrt_vec_mult(&path, dist);
	ray->hit = (t_hit){
		.hit = MRT_TRUE,
		.dist = dist,
		.color = obj->mat.obj,
		.point = ray->origin,
		.obj = obj};
	mrt_vec_add(&ray->hit.point, path);
}

void	mrt_sphere_inter(t_ray *ray, t_object *obj)
{
	t_mrt_vec	c_to_r;
	t_f32		params[6] = {0};
	t_f32		dist;

	c_to_r = ray->origin;
	mrt_vec_sub(&c_to_r, obj->pos);
	params[PARAM_A] = mrt_vec_dot(ray->direction, ray->direction);
	params[PARAM_B] = 2. * mrt_vec_dot(c_to_r, ray->direction);
	params[PARAM_C] = mrt_vec_dot(c_to_r, c_to_r) - powf(obj->data[MRT_OBJ_SPHERE_DIAMETER] / 2., 2);
	dist = 0;
	mrt_solve_poly((t_f32 *)params, &dist);
	if (dist > 0 && dist < ray->hit.dist)
		mrt_ray_update(ray, obj, dist);
}

void	mrt_plane_inter(t_ray *ray, t_object *obj)
{
	const t_mrt_vec	norm = obj->norm;
	t_mrt_vec		from;
	t_f32			d;
	t_f32			t;

	d = mrt_vec_dot(norm, ray->direction);
	if (mrt_fabs(d) > MRT_EPS)
	{
		from = obj->pos;
		mrt_vec_sub(&from, ray->origin);
		t = mrt_vec_dot(from, norm) / d;
		if (mrt_fabs(t) > MRT_EPS && t >= 0)
		{
			if (t >= ray->hit.dist)
				return ;
			mrt_ray_update(ray, obj, t);
		}
	}
}

t_mrt_color	mrt_color_mult(t_mrt_color c, t_f32 f)
{
	return ((t_mrt_color){
		.a = 255,
		.r = ((c.r / 255.) * f) * 255,
		.g = ((c.g / 255.) * f) * 255,
		.b = ((c.b / 255.) * f) * 255});
}

static void	mrt_ray_cast(t_scene *scene, t_ray *ray)
{
	t_object	*obj;
	t_u32		i = 0;

	while (i < scene->nobj)
	{
		obj = &scene->objects->objs[i];
		if (obj->type == MRT_OBJ_SPHERE)
			mrt_sphere_inter(ray, obj);
		if (obj->type == MRT_OBJ_PLANE)
			mrt_plane_inter(ray, obj);
		i++;
	}
}

static t_mrt_color	mrt_ray_color_ambient(t_scene *scene)
{
	t_object	*ambient;
	t_mrt_color	ambi;

	ambient = &scene->config[MRT_OBJ_AMBIENT];
	ambi = mrt_color_mult(ambient->mat.obj, ambient->mat.obj_r);
	return (ambi);
}

static t_mrt_color	mrt_ray_color_diffuse(t_scene *scene, t_ray *ray)
{
	const t_object	*light = &scene->config[MRT_OBJ_LIGHT];
	t_mrt_vec	lightpath;
	t_mrt_vec	norm;
	t_f32		dratio;

	light = &scene->config[MRT_OBJ_LIGHT];
	lightpath = light->pos;
	mrt_vec_sub(&lightpath, ray->hit.point);
	if (ray->hit.obj->type == MRT_OBJ_SPHERE)
	{
		norm = ray->hit.point;
		mrt_vec_sub(&norm, ray->hit.obj->pos);
		if (fabs(mrt_vec_dot(norm, ray->direction)) < 1.e-4)
			mrt_vec_mult(&norm, -1.);
	}
	if (ray->hit.obj->type == MRT_OBJ_PLANE)
		norm = ray->hit.obj->norm;
	mrt_vec_norm(&norm);
	mrt_vec_norm(&lightpath);
	dratio = fmaxf(mrt_vec_dot(lightpath, norm), 0.);
	dratio = fminf(light->mat.obj_r * dratio, 1.);
	return (mrt_color_mult(ray->hit.obj->mat.obj, dratio));
}

static void	mrt_ray_color(t_scene *scene, t_ray *ray)
{
	t_mrt_color	ambi;
	t_mrt_color	diff;

	ambi = mrt_ray_color_ambient(scene);
	diff = mrt_ray_color_diffuse(scene, ray);
	ray->hit.color = (t_mrt_color){
		.a = 255,
		.r = mrt_clamp(ambi.r + diff.r, 0, 255),
		.g = mrt_clamp(ambi.g + diff.g, 0, 255),
		.b = mrt_clamp(ambi.b + diff.b, 0, 255)};
}

static t_mrt_color	mrt_scene_get_color(t_scene *scene, t_u32 x, t_u32 y)
{
	static t_ray	ray[MRT_H * MRT_W] = {0};
	t_ray			*this;

	mrt_bzero(&ray, sizeof(t_ray));
	this = &ray[y * MRT_W + x];
	if (this->direction.w == 0.)
		mrt_ray_init(scene, this, x, y);
	mrt_ray_cast(scene, this);
	if (this->hit.obj)
		mrt_ray_color(scene, this);
	return (this->hit.color);
}

#define TIMER_GET 0
#define TIMER_SET 1

#include <sys/time.h>
static void	mrt_timer(t_u32 action)
{
	static struct timeval	tv = {0};
	struct timeval			ntv;

	if (!tv.tv_sec)
		gettimeofday(&tv, NULL);
	gettimeofday(&ntv, NULL);
	if (action == TIMER_SET)
		tv = ntv;
	else if (action == TIMER_GET)
	{
		if (ntv.tv_usec > tv.tv_usec)
			printf("\033c\rElapsed time: %zu.", ntv.tv_sec - tv.tv_sec);
		for (int i = 0; i < (int)(6 - log10f(ntv.tv_usec - tv.tv_usec)); i++)
			printf("0");
		printf("%zu\n", ntv.tv_usec - tv.tv_usec);
	}
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
	mrt_timer(TIMER_SET);
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
	mrt_timer(TIMER_GET);
	return (MRT_SUCCESS);
}
