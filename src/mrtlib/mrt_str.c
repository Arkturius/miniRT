// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_str.c                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/16 21:13:34 by rgramati          #+#    #+#             //
//   Updated: 2024/08/18 21:15:01 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <mrtlib.h>

#define HIGHS	0x8080808080808080
#define ONES	0x0101010101010101

size_t	mrt_strlen(const char *str)
{
	t_u8	*byte_ptr;
	t_u64	*long_ptr;

	byte_ptr = (t_u8 *)str;
	while ((t_uptr)byte_ptr & (sizeof(t_u64) - 1))
	{
		if (!*byte_ptr)
			return (byte_ptr - (t_u8 *)str);
		++byte_ptr;
	}
	long_ptr = (t_u64 *)byte_ptr;
	while (!((*long_ptr - ONES) & ~(*long_ptr) & HIGHS))
		++long_ptr;
	byte_ptr = (t_u8 *)long_ptr;
	while (*byte_ptr)
		++byte_ptr;
	return (byte_ptr - (t_u8 *)str);
}
