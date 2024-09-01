/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 21:13:34 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/02 00:25:12 by rems             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include <mrtlib.h>

#define HIGHS	0x8080808080808080
#define ONES	0x0101010101010101

/**
 * @todo	move those functions away, maybe i need more of those
 */
__attribute__((always_inline)) inline t_u32	mrt_is_aligned(void *p, t_u32 size)
{
	return (!(t_uptr)p & (size - 1));
}

__attribute__((always_inline)) inline t_u32	mrt_has_zeros(t_u64 l, char c)
{
	if (c)
		l ^= ONES * c;
	return (((l - ONES) & ~(l) & HIGHS));
}

size_t	mrt_strlen(const char *str)
{
	t_u8	*byte_ptr;
	t_u64	*long_ptr;

	byte_ptr = (t_u8 *)str;
	while (!mrt_is_aligned(byte_ptr, sizeof(t_u64)))
	{
		if (!*byte_ptr)
			return (byte_ptr - (t_u8 *)str);
		++byte_ptr;
	}
	long_ptr = (t_u64 *)byte_ptr;
	while (!mrt_has_zeros(*long_ptr, 0))
		++long_ptr;
	byte_ptr = (t_u8 *)long_ptr;
	while (*byte_ptr)
		++byte_ptr;
	return (byte_ptr - (t_u8 *)str);
}

int	mrt_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (s1 != s2);
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

char	*mrt_strchr(const char *str, char c)
{
	t_u8	*byte_ptr;
	t_u64	*long_ptr;

	byte_ptr = (t_u8 *)str;
	while (*byte_ptr != c && !mrt_is_aligned(byte_ptr, sizeof(t_u64)))
	{
		if (!*byte_ptr)
			return (NULL);
		++byte_ptr;
	}
	if (*byte_ptr == c)
		return ((char *)byte_ptr);
	long_ptr = (t_u64 *)byte_ptr;
	while (!mrt_has_zeros(*long_ptr, c))
		++long_ptr;
	byte_ptr = (t_u8 *)long_ptr;
	while (*byte_ptr && *byte_ptr != c)
		++byte_ptr;
	if (!byte_ptr)
		return (NULL);
	return ((char *)byte_ptr);
}

char	*mrt_strrchr(const char *str, char c)
{
	t_u8	*byte_ptr;
	t_u64	*long_ptr;

	byte_ptr = (t_u8 *)str + mrt_strlen(str) - 1;
	while (*byte_ptr != c && !mrt_is_aligned(byte_ptr, sizeof(t_u64)))
	{
		if (byte_ptr == (t_u8 *)str)
			return (NULL);
		--byte_ptr;
	}
	if (*byte_ptr == c)
		return ((char *)byte_ptr);
	long_ptr = (t_u64 *)byte_ptr;
	while ((t_u8 *)long_ptr - (t_u8 *)str > 8 && !mrt_has_zeros(*long_ptr, c))
		--long_ptr;
	byte_ptr = (t_u8 *)long_ptr;
	while (*byte_ptr != c)
	{
		if (byte_ptr == (t_u8 *)str)
			return (NULL);
		--byte_ptr;
	}
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
