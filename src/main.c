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

#include <unistd.h>

#include <SDL2/SDL_scancode.h>

#include <mlx.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>
#include <mrt/parser.h>

#define SIZE(X) printf("size of %s is %zu\n", #X, sizeof(X));

static void	mrt_file_clean(t_file *file)
{
	free(file->data);
}

char *mrt_file_select(void);

t_s32	main(__attribute__((unused)) int argc, char **argv)
{
	char		*name = mrt_file_select();
	t_file		file;
	t_scene		scene;
	t_error		err;

	mrt_bzero(&file, sizeof(t_file));
	mrt_bzero(&scene, sizeof(t_scene));
	err = mrt_parse_file(name, &file);
	if (err == MRT_SUCCESS)
		err = mrt_scene_init(&file, &scene);
	mrt_file_clean(&file);
	if (err == MRT_SUCCESS)
		err = mrt_mlx_init(&scene.mlx);
	// MRT_END_IMPLEMENTATION
	if (err == MRT_SUCCESS)
	{
		mrt_mlx_hook_setup(&scene);
		mrt_scene_render(&scene);
		mlx_loop(scene.mlx.app);
	}
	mrt_scene_clean(&scene);
	return (err);
}

