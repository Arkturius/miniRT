/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_parse_prim.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:45:13 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/19 14:33:21 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

t_errtype	mrt_parse_int(int *res, char *str, char **remain)
{
	int32_t	i;
	int32_t	err;

	i = mrt_strtoi(str, &str, &err);
	if (err)
		return (err);
	*res = i;
	*remain = str;
	return (MRT_SUCCESS);
}

t_errtype	mrt_parse_float(float *res, char *str, char **remain)
{
	float	f;
	int32_t	err;

	f = mrt_strtof(str, &str, &err);
	if (err)
		return (err);
	*res = f;
	*remain = str;
	return (MRT_SUCCESS);
}

t_errtype	mrt_parse_vec(t_mrt_vec *res, char *str, char **remain)
{
	t_mrt_vec	v;

	while (1)
	{
		if (mrt_parse_float(&v.x, str, &str))
			break ;
		if (*str++ != ',' || mrt_parse_float(&v.y, str, &str))
			break ;
		if (*str++ != ',' || mrt_parse_float(&v.z, str, &str))
			break ;
		v.w = 0;
		*res = v;
		while (mrt_isspace(*str))
			++str;
		*remain = str;
		return (MRT_SUCCESS);
	}
	return (MRT_FAIL);
}

t_errtype	mrt_parse_color(t_mrt_color *res, char *str, char **remain)
{
	t_mrt_color	c;
	int			tmp[3];

	while (1)
	{
		if (mrt_parse_int(tmp, str, &str))
			break ;
		if (*str++ != ',' || mrt_parse_int(tmp + 1, str, &str))
			break ;
		if (*str++ != ',' || mrt_parse_int(tmp + 2, str, &str))
			break ;
		c = (t_mrt_color){.a = 255, .r = (uint8_t) *(tmp),
			.g = (uint8_t) *(tmp + 1),
			.b = (uint8_t) *(tmp + 2)};
		*res = c;
		while (mrt_isspace(*str))
			++str;
		*remain = str;
		return (MRT_SUCCESS);
	}
	return (MRT_FAIL);
}
