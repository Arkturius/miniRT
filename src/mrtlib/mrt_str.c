/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 21:13:34 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/18 21:15:01 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

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

char	*mrt_strchr(const char *str, char c)
{
	t_u8	*byte_ptr;
	t_u64	*long_ptr;
	t_u64	c_mask;

	byte_ptr = (t_u8 *)str;
	while (*byte_ptr != c && (t_uptr)byte_ptr & (sizeof(t_u64) - 1))
	{
		if (!*byte_ptr)
			return (NULL);
		++byte_ptr;
	}
	if (*byte_ptr == c)
		return ((char *)byte_ptr);
	c_mask = ONES * c;
	long_ptr = (t_u64 *)byte_ptr;
	while (!(((*long_ptr ^ c_mask) - ONES) & ~((*long_ptr ^ c_mask)) & HIGHS))
		++long_ptr;
	byte_ptr = (t_u8 *)long_ptr;
	while (*byte_ptr && *byte_ptr != c)
		++byte_ptr;
	return ((char *)byte_ptr);
}

char	*mrt_strdup(const char *str)
{
	size_t	size;
	char	*dup;

	size = mrt_strlen(str);
	dup = malloc((size + 1) * sizeof(char));
	if (dup)
		mrt_memcpy(dup, str, size);
	return (dup);
}
