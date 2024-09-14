/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 18:26:02 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/18 21:16:26 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include <mrtlib.h>
#include <mrt/error.h>

#define MRT_ERR_MSG	"\033[38;2;255;0;0;1m[ERROR]\033[0m %s"

t_error	mrt_error_print(t_error err)
{
	static char	*msgs[9] = {MRT_MSG_ALLOC, MRT_MSG_MLX, MRT_MSG_FILE_NONE,
		MRT_MSG_FILE_EXTE, MRT_MSG_FILE_PERM, MRT_MSG_FILE_PROC,
		MRT_MSG_FMT_TYPE, MRT_MSG_FMT_CONFIG, MRT_MSG_FMT_DATA};

	if (err.type != MRT_SUCCESS)
	{
		err.type -= MRT_ERR_ALLOC;
		printf(MRT_ERR_MSG MRT_MSG_DATA_FMT, msgs[err.type], (char *)err.func);
	}
	return (err);
}
