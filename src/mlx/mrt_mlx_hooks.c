/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_mlx_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:33:21 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/18 21:15:58 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mrt/parser.h"
#include <SDL2/SDL_scancode.h>

#include <mlx.h>

#include <mrt/engine.h>

#define MOUSE_LEFT		1
#define MOUSE_MIDDLE	2
#define MOUSE_RIGHT		3

int	mrt_mlx_hook_mousedown(int key, void *scene_ptr)
{
	t_scene	*scene;
	t_ray	r;
	int32_t	x;
	int32_t	y;

	scene = (t_scene *)scene_ptr;
	if (key == MOUSE_LEFT)
	{
		mlx_mouse_get_pos(scene->mlx.app, &x, &y);
		mrt_ray_init(scene, &r, x, MRT_H - y);
		mrt_ray_cast(scene, &r);
		if (r.hit.obj)
			mrt_ray_color(scene, &r);
	}
	(void) scene;
	return (MRT_SUCCESS);
}

int	mrt_mlx_hook_keyup(int key, void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	(void)key;
	(void)scene;
	return (MRT_SUCCESS);
}

int	mrt_mlx_hook_keydown(int key, void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	if (key == SDL_SCANCODE_ESCAPE)
	{
		mlx_loop_end(scene->mlx.app);
		return (MRT_FAIL);
	}
	scene->config[0].data[0] = 1;
	return (MRT_SUCCESS);
}

int	mrt_mlx_hook_win(int key, void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	if (!key)
	{
		mlx_loop_end(scene->mlx.app);
		return (MRT_FAIL);
	}
	return (MRT_SUCCESS);
}

int	mrt_mlx_hook_loop(void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	if (scene->config[0].data[0])
	{
		printf("actually rendering\n");
		mrt_scene_render(scene);
		printf("rendered !\n");
	}
	return (MRT_SUCCESS);
}
