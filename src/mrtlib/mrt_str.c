// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_str.c                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/16 21:13:34 by rgramati          #+#    #+#             //
//   Updated: 2024/08/16 21:34:50 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <mrtlib.h>

size_t	mrt_strlen(const char *str)
{
	char	*tmp;

	tmp = (char *)str;
	while (*tmp)
		tmp++;
	return (tmp - str);
}
