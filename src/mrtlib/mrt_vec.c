/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_vec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:30:38 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/07 14:38:52 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mrtlib.h>

void	mrt_vec_sub(t_mrt_vec v1, t_mrt_vec v2, t_mrt_vec *n)
{
	const t_mrt_vec	ret = (t_mrt_vec){
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z,
		.w = 0};

	*n = ret;
}

void	mrt_vec_add(t_mrt_vec v1, t_mrt_vec v2, t_mrt_vec *n)
{
	const t_mrt_vec	ret = (t_mrt_vec){
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z,
		.w = 0};

	*n = ret;
}

void	mrt_vec_mult(t_mrt_vec v1, float f, t_mrt_vec *n)
{
	const t_mrt_vec	ret = (t_mrt_vec){
		.x = v1.x * f,
		.y = v1.y * f,
		.z = v1.z * f,
		.w = 0};

	*n = ret;
}
