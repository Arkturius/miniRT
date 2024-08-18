/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:01:46 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/13 20:30:59 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include <SDL2/SDL_scancode.h>

#include <mlx.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>
#include <mrt/parser.h>

static void	mrt_mlx_scene_render(t_mlx *mlx, t_scene *scene)
{
	(void) scene;
	mlx_loop(mlx->app);
}

static t_error	mrt_scene_convert(t_parser *parser, t_scene *scene)
{
	scene->n_obj = parser->obj_count;
	mrt_pobj_clean(parser->objs);
	return (MRT_SUCCESS);
}

static void	mrt_scene_clean(t_scene *scene)
{
	(void) scene;
}

t_s32	main(__attribute__((unused)) int argc, char **argv)
{
	t_mlx		mlx;
	t_parser	parser;
	t_scene		scene;
	t_error		ret;

	ret = MRT_FAIL;
	mrt_bzero(&mlx, sizeof(t_mlx));
	mrt_bzero(&parser, sizeof(t_parser));
	mrt_bzero(&scene, sizeof(t_scene));
	while (1)
	{
		if (mrt_parse_file(argv[1], &parser))
			break ;
		if (mrt_scene_convert(&parser, &scene))
			break ;
		if (mrt_mlx_init(&mlx))
			break ;
		mrt_mlx_hook_setup(&mlx);
		mrt_mlx_scene_render(&mlx, &scene);
		ret = MRT_SUCCESS;
		break ;
	}
	mrt_scene_clean(&scene);
	mrt_mlx_clean(&mlx);
	return (ret);
}
