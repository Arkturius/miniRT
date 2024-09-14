/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_objects.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:27:05 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/07 14:47:47 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>

void	mrt_solve_poly(float *params, float *res)
{
	float	dist;

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

void	mrt_ray_update(t_ray *ray, t_object *obj, float dist)
{
	t_mrt_vec	path;

	mrt_vec_mult(ray->direction, dist, &path);
	ray->hit = (t_hit){
		.dist = dist,
		.color = obj->mat.obj,
		.obj = obj};
	mrt_vec_add(ray->origin, path, &ray->hit.point);
}

void	mrt_sphere_inter(t_ray *ray, t_object *obj)
{
	t_mrt_vec	c_to_r;
	float		params[6];
	float		dist;

	mrt_bzero(params, sizeof(params));
	mrt_vec_sub(ray->origin, obj->pos, &c_to_r);
	params[PARAM_A] = mrt_vec_dot(ray->direction, ray->direction);
	params[PARAM_B] = 2. * mrt_vec_dot(c_to_r, ray->direction);
	params[PARAM_C] = mrt_vec_dot(c_to_r, c_to_r) \
		- powf(obj->data[MRT_OBJ_SPHERE_DIAMETER] / 2., 2);
	dist = 0;
	mrt_solve_poly((float *)params, &dist);
	if (dist > 0 && dist < ray->hit.dist)
		mrt_ray_update(ray, obj, dist);
}

void	mrt_plane_inter(t_ray *ray, t_object *obj)
{
	const t_mrt_vec	norm = obj->norm;
	t_mrt_vec		from;
	float			d;
	float			t;

	d = mrt_vec_dot(norm, ray->direction);
	if (mrt_fabs(d) > MRT_EPS)
	{
		mrt_vec_sub(obj->pos, ray->origin, &from);
		t = mrt_vec_dot(from, norm) / d;
		if (mrt_fabs(t) > MRT_EPS && t >= 0)
		{
			if (t >= ray->hit.dist)
				return ;
			mrt_ray_update(ray, obj, t);
		}
	}
}
