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

t_f32	mrt_fabs(t_f32 f)
{
	if (f < 0)
		return (-f);
	return (f);
}

t_u32	mrt_min(t_u32 a, t_u32 b)
{
	if (a < b)
		return (a);
	return (b);
}

t_u32	mrt_max(t_u32 a, t_u32 b)
{
	if (a >= b)
		return (a);
	return (b);
}

t_u32	mrt_clamp(t_u32 f, t_u32 cmin, t_u32 cmax)
{
	return (mrt_min(mrt_max(f, cmin), cmax));
}
