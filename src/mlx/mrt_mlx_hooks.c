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

#include <SDL2/SDL_scancode.h>

#include <mlx.h>

#include <mrt/engine.h>

int	mrt_mlx_hook_mousedown(int key, void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	printf("%d\n", key);
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
