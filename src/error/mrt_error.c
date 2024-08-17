// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_error.c                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/15 18:26:02 by rgramati          #+#    #+#             //
//   Updated: 2024/08/16 21:11:46 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdio.h>

#include <mrtlib.h>
#include <mrt/error.h>

#define MRT_ERROR	"\033[31;1m/!\\ [Error] > \033[39;21;3m%s\n"

t_error	mrt_error_print(t_error type, void *data)
{
	(void) data;

	if (type != MRT_SUCCESS)
		printf(MRT_ERROR, (char *)data);
	return (type);
}
