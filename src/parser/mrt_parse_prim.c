// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_parse_prim.c                                   :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/17 20:45:13 by rgramati          #+#    #+#             //
//   Updated: 2024/08/17 21:01:47 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

t_error	mrt_parse_int(int *res, char *str, char **remain)
{
	t_s32	i;
	t_error	err;
	char	*tmp;

	tmp = str;
	i = mrt_strtoi(str, remain, &err);
	if (err)
	{
		*remain = tmp;
		return (err);
	}
	*res = i;
	return (MRT_SUCCESS);
}

t_error	mrt_parse_float(float *res, char *str, char **remain)
{
	t_f32	f;
	t_error	err;
	char	*tmp;

	tmp = str;
	f = mrt_strtof(str, remain, &err);
	if (err)
	{
		*remain = tmp;
		return (err);
	}
	*res = f;
	return (MRT_SUCCESS);
}

t_error	mrt_parse_vec(t_mrt_vec *res, char *str, char **remain)
{
	t_mrt_vec	v;
	char		*tmp;

	tmp = str;
	while (1)
	{
		if (mrt_parse_float(&v.x, str, remain))
			break ;
		if (mrt_parse_float(&v.y, str, remain))
			break ;
		if (mrt_parse_float(&v.z, str, remain))
			break ;
		v.w = 0;
		*res = v;
		return (MRT_SUCCESS);
	}
	*remain = tmp;
	return (MRT_FAIL);
}

t_error mrt_parse_color(t_mrt_color *res, char *str, char **remain)
{
	t_mrt_color	c;
	char		*tmp;

	tmp = str;
	while (1)
	{
		if (mrt_parse_int((int *)&c.r, str, remain))
			break ;
		if (mrt_parse_int((int *)&c.g, str, remain))
			break ;
		if (mrt_parse_int((int *)&c.b, str, remain))
			break ;
		c.a = 255;
		*res = c;
		return (MRT_SUCCESS);
	}
	*remain = tmp;
	return (MRT_FAIL);
}
