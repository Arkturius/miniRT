// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   error.h                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/15 18:26:23 by rgramati          #+#    #+#             //
//   Updated: 2024/08/16 21:19:45 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef ERROR_H
# define ERROR_H

# include <mrtlib.h>

# define MRT_END_IMPLEMENTATION	exit(0);

typedef enum e_mrt_error
{
	MRT_SUCCESS,
	MRT_FAIL,
	MRT_ERR_ALLOC,
	MRT_ERR_MLX,
	MRT_ERR_FILE,
}	t_error;

t_error	mrt_error_print(t_error type, void *data);

#endif
