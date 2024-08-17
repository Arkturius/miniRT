// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_mlx.c                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/13 19:29:29 by rgramati          #+#    #+#             //
//   Updated: 2024/08/16 21:33:38 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <mlx.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>

void	mrt_mlx_clean(t_mlx *mlx)
{
	if (mlx->app)
		mlx_destroy_image(mlx->app, mlx->img);
	if (mlx->win)
		mlx_destroy_window(mlx->app, mlx->win);
	if (mlx->img)
		mlx_destroy_display(mlx->app);
}

t_error	mrt_mlx_init(t_mlx *mlx)
{
	char	*err;

	while (1)
	{
		err = "Failed to create instance...";
		mlx->app = mlx_init();
		if (!mlx->app)
			break ;
		err = "Failed to create window...";
		mlx->win = mlx_new_window(mlx->app, MRT_W, MRT_H, "miniRT");
		if (!mlx->win)
			break ;
		err = "Failed to create image...";
		mlx->img = mlx_new_image(mlx->app, MRT_W, MRT_H);
		if (!mlx->img)
			break ;
		return (MRT_SUCCESS);
	}
	mrt_mlx_clean(mlx);
	return (mrt_error_print(MRT_ERR_MLX, (void *)err));
}

t_u32	mrt_mlx_hook_setup(t_mlx *mlx)
{
	if (!mlx)
		return (MRT_FAIL);
	mlx_on_event(mlx->app, mlx->win, MLX_KEYDOWN, mrt_mlx_hook_keydown, mlx);
	mlx_on_event(mlx->app, mlx->win, MLX_WINDOW_EVENT, mrt_mlx_hook_window, mlx);
	return (MRT_SUCCESS);
}
