/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:32:26 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/02 00:22:32 by rems             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <unistd.h>

#include <SDL2/SDL_scancode.h>

#include <mlx.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>
#include <mrt/parser.h>

static void	mrt_file_clean(t_file *file)
{
	free(file->data);
}

int	main(__attribute__((unused)) int argc, char **argv)
{
	t_file		file;
	t_scene		scene;
	t_error		err;

	mrt_bzero(&file, sizeof(t_file));
	mrt_bzero(&scene, sizeof(t_scene));
	err = mrt_parse_file(argv[1], &file);
	if (err.type == MRT_SUCCESS)
		err = mrt_scene_init(&file, &scene);
	mrt_file_clean(&file);
	if (err.type == MRT_SUCCESS)
		err = mrt_mlx_init(&scene.mlx);
	if (err.type == MRT_SUCCESS)
	{
		mrt_mlx_hook_setup(&scene);
		mrt_scene_render(&scene);
		mlx_loop(scene.mlx.app);
	}
	mrt_obj_chunk_clean(scene.objects);
	mrt_mlx_clean(scene.mlx);
	mrt_error_print(err);
	return (err.type);
}
