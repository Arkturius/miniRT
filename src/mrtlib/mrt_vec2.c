/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_vec2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:31:15 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/07 14:51:03 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include <mrtlib.h>

t_f32	mrt_vec_len(t_mrt_vec v)
{
	const t_f32	len = sqrtf(powf(v.x, 2.) + powf(v.y, 2.) + powf(v.z, 2.));

	return (len);
}

void	mrt_vec_norm(t_mrt_vec v, t_mrt_vec *n)
{
	const t_f32		len = mrt_vec_len(v);
	const t_mrt_vec	ret = {.x = v.x / len, \
							.y = v.y / len, \
							.z = v.z / len};

	*n = ret;
}

t_f32	mrt_vec_dot(t_mrt_vec v1, t_mrt_vec v2)
{
	const t_f32	dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return (dot);
}

void	mrt_vec_cross(t_mrt_vec v1, t_mrt_vec v2, t_mrt_vec *n)
{
	const t_mrt_vec	ret = (t_mrt_vec){
		.x = v1.y * v2.z - v2.y * v1.z,
		.y = v1.z * v2.x - v1.x * v2.z,
		.z = v1.x * v2.y - v2.x * v1.y,
		.w = 0};

	*n = ret;
}
