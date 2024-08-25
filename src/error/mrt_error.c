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

#define MRT_ERROR	"\033[31;1m/!\\ [Error] > \033[39;21;3m%s\n"

t_error	mrt_error_alloc(const char *func)
{
	printf(MRT_ERROR, func);
	return (MRT_ERR_ALLOC);
}

t_error	mrt_error_print(t_error type, void *data)
{
	if (type != MRT_SUCCESS)
		printf(MRT_ERROR, (char *)data);
	return (type);
}
