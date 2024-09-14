/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:38:15 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/07 12:40:39 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mrtlib.h>

float	mrt_fabs(float f)
{
	if (f < 0)
		return (-f);
	return (f);
}

uint32_t	mrt_min(uint32_t a, uint32_t b)
{
	if (a < b)
		return (a);
	return (b);
}

uint32_t	mrt_max(uint32_t a, uint32_t b)
{
	if (a >= b)
		return (a);
	return (b);
}

uint32_t	mrt_clamp(uint32_t f, uint32_t cmin, uint32_t cmax)
{
	return (mrt_min(mrt_max(f, cmin), cmax));
}
