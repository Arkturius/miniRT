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

#include <mlx.h>
#include <SDL2/SDL_scancode.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>

#ifndef MRT_BONUS

static t_mrt_color	mrt_scene_get_color(t_scene *scene, uint32_t x, uint32_t y)
{
	static t_ray	ray[MRT_H * MRT_W] = {0};
	const t_mrt_vec	campos = scene->config[MRT_OBJ_CAMERA].pos;
	t_ray			*this;

	mrt_bzero(&ray, sizeof(t_ray));
	this = &ray[y * MRT_W + x];
	if (this->direction.w == 0.)
		mrt_ray_init(scene, this, x, y);
	this->origin = campos;
	this->hit.obj = NULL;
	this->hit.dist = INFINITY;
	mrt_ray_cast(scene, this);
	if (this->hit.obj)
		mrt_ray_color(scene, this);
	return (this->hit.color);
}

#else

static t_mrt_color	mrt_scene_get_color(t_scene *scene, uint32_t x, uint32_t y)
{
	(void)scene;
	(void)x;
	(void)y;
	return ((t_mrt_color){.argb = 0xFFF0F0F0});
}

#endif

int	mrt_scene_render(void *scene_ptr)
{
	uint32_t	x;
	uint32_t	y;
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
	scene->config[0].data[0] = 0;
	return (MRT_SUCCESS);
}
