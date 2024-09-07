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

int	mrt_mlx_hook_mousedown(int key, void *scene_ptr)
{
	t_scene	*scene;
	t_ray	r;
	t_s32	x;
	t_s32	y;

	scene = (t_scene *)scene_ptr;
	if (key == 1)
	{
		mlx_mouse_get_pos(scene->mlx.app, &x, &y);
		mrt_ray_init(scene, &r, x, MRT_H - y);
		mrt_ray_cast(scene, &r);
		if (r.hit.obj)
			mrt_ray_color(scene, &r);
	}
	(void) scene;
	return (0);
}

int	mrt_mlx_hook_keyup(int key, void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	if (scene->map[key])
		scene->map[key] = 0;
	return (0);
}

int	mrt_mlx_hook_keydown(int key, void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	if (key == SDL_SCANCODE_ESCAPE)
		mlx_loop_end(scene->mlx.app);
	if (!scene->map[key])
		scene->map[key] = 1;
	if (key == SDL_SCANCODE_D)
		scene->config[MRT_OBJ_CAMERA].pos.z -= 0.2;
	if (key == SDL_SCANCODE_A)
		scene->config[MRT_OBJ_CAMERA].pos.z += 0.2;
	scene->config[MRT_OBJ_CAMERA].pos.w = 0;
	mrt_scene_render(scene);
	return (0);
}

int	mrt_mlx_hook_win(int key, void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	if (!key)
		mlx_loop_end(scene->mlx.app);
	return (0);
}
