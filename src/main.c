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
#include <stdio.h> // TO REMOVE, include my printf

#include <SDL2/SDL_scancode.h>

#include <mlx.h>

#define MRTLIB_IMPLEMENTATION 1

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>
#include <mrt/parser.h>

t_s32	main(int argc, char **argv)
{
	(void) argc;

	t_mlx		application = {.app = NULL, .win = NULL, .img = NULL};
	t_parser	parser;
	t_u32		ret;

	ret = EXIT_FAILURE;
	if (mrt_parse_file(argv[1], &parser))
		return (ret);
	if (!mrt_mlx_init(&application))
	{
		mrt_mlx_hook_setup(&application);
		mlx_loop(application.app);
		ret = EXIT_SUCCESS;
	}
	mrt_mlx_clean(&application);
	return (ret);
}
