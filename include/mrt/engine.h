// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   engine.h                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/13 19:02:24 by rgramati          #+#    #+#             //
//   Updated: 2024/08/17 20:33:32 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef ENGINE_H
# define ENGINE_H

# include <mrtlib.h>

# define MRT_H	768
# define MRT_W	1367

typedef struct	s_mlx
{
	void	*app;
	void	*win;
	void	*img;
}	t_mlx;

typedef enum e_mrt_error	t_error;

// MLX FUNCS **************************************************************** //

t_error	mrt_mlx_init(t_mlx *mlx);

void	mrt_mlx_clean(t_mlx *mlx);

t_error	mrt_mlx_hook_setup(t_mlx *mlx);

// MLX HOOKS **************************************************************** //

t_s32	mrt_mlx_hook_keyup(t_s32 key, void *mlx);

t_s32	mrt_mlx_hook_keydown(t_s32 key, void *mlx);

t_s32	mrt_mlx_hook_window(t_s32 key, void *mlx);

// ************************************************************************** //

#endif
