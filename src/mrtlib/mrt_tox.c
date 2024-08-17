// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_tox.c                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/17 21:02:12 by rgramati          #+#    #+#             //
//   Updated: 2024/08/17 21:51:40 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <mrtlib.h>

int	mrt_isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int	mrt_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

// only handles base 10 for now, TODO: base argument
int	mrt_strtoi(char *str, char **remain, int *err)
{
	int	res;
	int tmp;
	int sign;

	res = 0;
	sign = 0;
	*err = MRT_SUCCESS;
	while (mrt_isspace((unsigned char)*str))
		++str;
	if (!((*str - '+') & ~2))
		sign = (*str == '-');
	++str;
	while (*str && mrt_isdigit((unsigned char)*str))
	{
		tmp = res * 10 + (*str - '0');
		if (tmp < res)
			*err = MRT_FAIL;
		res = tmp;
	}
	*remain = str;
	if (sign)
		return (-res);
	return (res);
}
