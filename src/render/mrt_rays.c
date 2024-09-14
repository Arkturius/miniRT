/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_rays.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:43:05 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/07 14:43:59 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>

void	mrt_ray_color(t_scene *scene, t_ray *ray)
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

void	mrt_ray_cast(t_scene *scene, t_ray *ray)
{
	t_object	*obj;
	uint32_t	i;

	i = 0;
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

void	mrt_ray_init(t_scene *scene, t_ray *ray, uint32_t x, uint32_t y)
{
	const t_object	*cam = &scene->config[MRT_OBJ_CAMERA];
	const uint32_t	*fov_addr = (uint32_t *)&cam->data[MRT_OBJ_CAMERA_FOV];
	const double	ratio = tan(*fov_addr * 0.5 * MRT_PI / 180);
	const double	tr[4] = {cos(cam->norm.x), cos(cam->norm.y), \
								sin(cam->norm.x), sin(cam->norm.y)};
	t_mrt_vec		uv;

	uv.x = (ratio * ((2. * x - MRT_W) / MRT_H));
	uv.y = (ratio * ((2. * y - MRT_H) / MRT_H));
	uv.z = 1;
	mrt_vec_norm(uv, &uv);
	ray->direction = (t_mrt_vec){
		.x = uv.x * tr[1] + uv.y * tr[2] * tr[3] - uv.z * tr[0] * tr[3],
		.y = uv.y * tr[0] + uv.z * tr[2],
		.z = uv.x * tr[3] - uv.y * tr[2] * tr[1] + uv.z * tr[0] * tr[1],
		.w = 1};
	ray->hit.color = (t_mrt_color){.a = 255};
}
