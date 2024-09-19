/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_colors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:47:55 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/07 14:43:22 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>

t_mrt_color	mrt_color_mult(t_mrt_color c, float f)
{
	return ((t_mrt_color){
		.a = 255,
		.r = ((c.r / 255.) * f) * 255,
		.g = ((c.g / 255.) * f) * 255,
		.b = ((c.b / 255.) * f) * 255});
}

t_mrt_color	mrt_ray_color_ambient(t_scene *scene)
{
	t_object	*ambient;
	t_mrt_color	ambi;

	ambient = &scene->config[MRT_OBJ_AMBIENT];
	ambi = mrt_color_mult(ambient->mat.obj, ambient->mat.obj_r);
	return (ambi);
}

#ifndef MRT_BONUS

t_mrt_color	mrt_ray_color_diffuse(t_scene *scene, t_ray *ray)
{
	const t_object	*light = &scene->config[MRT_OBJ_LIGHT];
	t_mrt_vec		lightpath;
	t_mrt_vec		norm;
	float			dratio;

	lightpath = light->pos;
	mrt_vec_sub(light->pos, ray->hit.point, &lightpath);
	if (ray->hit.obj->type == MRT_OBJ_SPHERE)
	{
		mrt_vec_sub(ray->hit.point, ray->hit.obj->pos, &norm);
		if (fabs(mrt_vec_dot(norm, ray->direction)) < MRT_EPS)
			mrt_vec_mult(norm, -1., &norm);
	}
	if (ray->hit.obj->type == MRT_OBJ_PLANE)
		norm = ray->hit.obj->norm;
	mrt_vec_norm(norm, &norm);
	mrt_vec_norm(lightpath, &lightpath);
	dratio = fmaxf(mrt_vec_dot(lightpath, norm), 0.);
	dratio = fminf(light->mat.obj_r * dratio, 1.);
	return (mrt_color_mult(ray->hit.obj->mat.obj, dratio));
}

#else

t_mrt_color	mrt_ray_color_diffuse(t_scene *scene, t_ray *ray)
{
	const t_object	*light = &scene->lights->objs[0];
	t_mrt_vec		lightpath;
	t_mrt_vec		norm;
	float			dratio;

	lightpath = light->pos;
	mrt_vec_sub(light->pos, ray->hit.point, &lightpath);
	if (ray->hit.obj->type == MRT_OBJ_SPHERE)
	{
		mrt_vec_sub(ray->hit.point, ray->hit.obj->pos, &norm);
		if (fabs(mrt_vec_dot(norm, ray->direction)) < MRT_EPS)
			mrt_vec_mult(norm, -1., &norm);
	}
	if (ray->hit.obj->type == MRT_OBJ_PLANE || ray->hit.obj->type == MRT_OBJ_OBJFILE)
		norm = ray->hit.obj->norm;
	mrt_vec_norm(norm, &norm);
	mrt_vec_norm(lightpath, &lightpath);
	dratio = fmaxf(mrt_vec_dot(lightpath, norm), 0.);
	dratio = fminf(light->mat.obj_r * dratio, 1.);
	return (mrt_color_mult(ray->hit.obj->mat.obj, dratio));
}

#endif
