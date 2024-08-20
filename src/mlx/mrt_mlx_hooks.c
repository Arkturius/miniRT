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

int	mrt_mlx_hook_keyup(int key, void *mlx_ptr)
{
	t_mlx	*mlx;

	mlx = (t_mlx *)mlx_ptr;
	(void) mlx;
	(void) key;
	return (0);
}

int	mrt_mlx_hook_keydown(int key, void *mlx_ptr)
{
	t_mlx	*mlx;

	mlx = (t_mlx *)mlx_ptr;
	if (key == SDL_SCANCODE_ESCAPE)
		mlx_loop_end(mlx->app);
	return (0);
}

int	mrt_mlx_hook_win(int key, void *mlx_ptr)
{
	t_mlx	*mlx;

	mlx = (t_mlx *)mlx_ptr;
	if (!key)
		mlx_loop_end(mlx->app);
	return (0);
}
