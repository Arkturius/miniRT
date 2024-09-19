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
#include "mrtlib.h"
#include <SDL2/SDL_scancode.h>

#include <mlx.h>

#include <mrt/engine.h>
#include <stdio.h>

#define MOUSE_LEFT		1
#define MOUSE_MIDDLE	2
#define MOUSE_RIGHT		3

int	mrt_mlx_hook_mousedown(int key, void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	(void) scene;
	(void) key;
	return (MRT_SUCCESS);
}

int	mrt_mlx_hook_keyup(int key, void *scene_ptr)
{
	t_scene	*scene;

	scene = (t_scene *)scene_ptr;
	(void)key;
	(void)scene;
	if (key == SDL_SCANCODE_W)
		scene->config[0].mat.obj.a = 0;
	if (key == SDL_SCANCODE_S)
		scene->config[0].mat.obj.r = 0;
	if (key == SDL_SCANCODE_SPACE)
		scene->config[0].mat.obj.g = 0;
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
	if (key == SDL_SCANCODE_W)
		scene->config[0].mat.obj.a = 1;
	if (key == SDL_SCANCODE_S)
		scene->config[0].mat.obj.r = 1;
	if (key == SDL_SCANCODE_SPACE)
		scene->config[0].mat.obj.g = 1;
		// scene->config[MRT_OBJ_CAMERA].pos.x += 0.2;
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

void mrt_jump(t_mrt_vec *pos, t_mrt_vec *velocity)
{
	static const float	g = -9.81f;
	static const float	dt = 0.016f;
    velocity->y += (1.3f * g) * dt;

    pos->x += velocity->x * dt;
    pos->y += velocity->y * dt;
    pos->z += velocity->z * dt;

    if (pos->y <= 0.0f) {
        pos->y = 0.0f;
        velocity->y = 0.0f;
    }
}

int	mrt_mlx_hook_loop(void *scene_ptr)
{
	static t_scene		*scene;
	static t_mrt_vec	*velocity = NULL;
	static t_mrt_vec	*campos = NULL;
	static t_bool		jumping = MRT_FALSE;

	scene = (t_scene *)scene_ptr;
	if (!velocity)
		velocity = &scene->config[0].norm;
	if (!campos)
		campos = &scene->config[MRT_OBJ_CAMERA].pos;
	if (scene->config[0].data[0])
	{
		printf("\033[%dA\r", MRT_H / 2);
		if (scene->config[0].mat.obj.a)
			scene->config[MRT_OBJ_CAMERA].pos.z += 0.1;
		if (scene->config[0].mat.obj.r)
			scene->config[MRT_OBJ_CAMERA].pos.z -= 0.1;
		if (scene->config[0].mat.obj.g && jumping == MRT_FALSE)
		{
			*velocity = (t_mrt_vec){0, 4, 0, 0};
			jumping = MRT_TRUE;
		}
		if (campos->y > 1.0f || velocity->y > 0.0f)
		{
			mrt_jump(campos, velocity);
			jumping = MRT_TRUE;
		}
		else
			jumping = MRT_FALSE;
		if (scene->config[0].mat.obj.argb || jumping)
			mrt_scene_term_render(scene);
		usleep(50);
	}
	return (MRT_SUCCESS);
}
