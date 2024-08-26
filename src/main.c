/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:32:26 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/20 18:33:01 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include <SDL2/SDL_scancode.h>

#include <mlx.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>
#include <mrt/parser.h>

void	mrt_mlx_scene_render(t_scene *scene)
{
	(void) scene;
	mlx_loop_hook(scene->mlx.app, mrt_scene_render, (void *)scene);
	mlx_loop(scene->mlx.app);
}

void	mrt_scene_cam_init(t_scene *scene)
{
	const t_mrt_vec	look = scene->config[MRT_OBJ_CAMERA].data.norm;
	const double	alpha = -acos(look.z / sqrt(pow(look.x, 2.) \
							+ pow(look.z, 2.)));
	const double	beta = atan2(look.y, look.x);

	scene->config[MRT_OBJ_CAMERA].data.norm = \
		(t_mrt_vec){.x = alpha, .y = beta, .z = 0, .w = 0};
}

t_error	mrt_scene_convert(t_file *file, t_scene *scene)
{
	mrt_parse_pobj_all(file);
	mrt_scene_obj_init(scene, file);
	mrt_scene_cam_init(scene);
	mrt_scene_aabb_init(scene);
	mrt_pobj_clean(file->objs);
	// mrt_io_save("save/objects.mrt", "@> ", scene->objects, 32, sizeof(t_object));
	mrt_io_save("save/camera.mrt", "@ ", &scene->config[MRT_OBJ_CAMERA].data, 1, sizeof(t_objD));
	return (MRT_SUCCESS);
}

void	mrt_scene_clean(t_scene *scene)
{
	mrt_obj_chunk_clean(scene->objects);
	mrt_mlx_clean(&scene->mlx);
	(void) scene;
}

#define SIZE(X) printf("size of %s is %zu\n", #X, sizeof(X));

t_s32	main(__attribute__((unused)) int argc, char **argv)
{
	t_file		file;
	t_scene		scene;
	t_error		ret;

	SIZE(t_ray);
	SIZE(t_objD);

	ret = MRT_FAIL;
	mrt_bzero(&file, sizeof(t_file));
	mrt_bzero(&scene, sizeof(t_scene));
	while (1)
	{
		if (mrt_parse_file(argv[1], &file))
			break ;
		if (mrt_scene_convert(&file, &scene))
			break ;
		if (mrt_mlx_init(&scene.mlx))
			break ;
		mrt_mlx_hook_setup(&scene.mlx);
		mrt_mlx_scene_render(&scene);
		ret = MRT_SUCCESS;
		break ;
	}
	mrt_scene_clean(&scene);
	return (ret);
}
