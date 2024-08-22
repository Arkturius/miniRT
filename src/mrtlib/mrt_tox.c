/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_tox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:02:12 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/18 17:27:41 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mrtlib.h>

int	mrt_strtoi(char *str, char **remain, int *err)
{
	int	res;
	int	tmp;
	int	sign;

	res = 0;
	sign = 0;
	*err = MRT_SUCCESS;
	while (mrt_isspace((unsigned char)*str))
		++str;
	sign = (*str == '-');
	str += !((*str - '+') & 253);
	while (mrt_isdigit((unsigned char)*str))
	{
		tmp = res * 10 + (*str - '0');
		if (tmp < res && tmp != (1 << 31))
			*err = MRT_FAIL;
		res = tmp;
		++str;
	}
	*err = (str == *remain);
	*remain = str;
	if (sign)
		return (-res);
	return (res);
}

float	mrt_strtof_comma(char **str)
{
	char	*tmp;

	tmp = *str;
	if (*tmp && mrt_isdigit(*tmp))
	{
		*str = tmp + 1;
		return ((*tmp - '0') + mrt_strtof_comma(str) / 10);
	}
	return (0);
}

float	mrt_strtof(char *str, char **remain, int *err)
{
	int		int_res;
	float	res;
	int		sign;

	res = 0.f;
	sign = 0.f;
	*err = MRT_SUCCESS;
	while (mrt_isspace((unsigned char)*str))
		++str;
	sign = (*str == '-');
	str += !((*str - '+') & 253);
	int_res = mrt_strtoi(str, &str, err);
	if (*err)
		return (MRT_FAIL);
	if (*str == '.')
	{
		++str;
		res = int_res + mrt_strtof_comma(&str) / 10;
		if (res < int_res)
			*err = MRT_FAIL;
	}
	*remain = str + (*str == 'f');
	if (sign)
		return (-res);
	return (res);
}
