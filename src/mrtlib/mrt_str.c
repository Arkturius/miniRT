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
static uint64_t	mrt_is_aligned(void *p, uint32_t size)
{
	return (!((uintptr_t)p & (size - 1)));
}

static uint64_t	mrt_has_zero(uint64_t l, char c)
{
	l ^= (ONES * c);
	return (((l - ONES) & ~(l) & HIGHS));
}

size_t	mrt_strlen(const char *str)
{
	uint8_t		*byte_ptr;
	uint64_t	*long_ptr;

	byte_ptr = (uint8_t *)str;
	while (!mrt_is_aligned(byte_ptr, sizeof(uint64_t)))
	{
		if (!*byte_ptr)
			return (byte_ptr - (uint8_t *)str);
		++byte_ptr;
	}
	long_ptr = (uint64_t *)byte_ptr;
	while (!mrt_has_zero(*long_ptr, 0))
		++long_ptr;
	byte_ptr = (uint8_t *)long_ptr;
	while (*byte_ptr)
		++byte_ptr;
	return (byte_ptr - (uint8_t *)str);
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
	uint8_t		*byte_ptr;
	uint64_t	*long_ptr;

	byte_ptr = (uint8_t *)str;
	while (*byte_ptr != c && !mrt_is_aligned(byte_ptr, sizeof(uint64_t)))
	{
		if (!*byte_ptr)
			return (NULL);
		++byte_ptr;
	}
	if (*byte_ptr == c)
		return ((char *)byte_ptr);
	long_ptr = (uint64_t *)byte_ptr;
	while (!mrt_has_zero(*long_ptr, 0) && !mrt_has_zero(*long_ptr, c))
		++long_ptr;
	byte_ptr = (uint8_t *)long_ptr;
	while (*byte_ptr && *byte_ptr != c)
		++byte_ptr;
	if (!byte_ptr)
		return (NULL);
	return ((char *)byte_ptr);
}

char	*mrt_strrchr(const char *str, char c)
{
	uint8_t		*byte_ptr;
	uint64_t	*long_ptr;

	byte_ptr = (uint8_t *)str + mrt_strlen(str) - 1;
	while (*byte_ptr != c && !mrt_is_aligned(byte_ptr, sizeof(uint64_t)))
	{
		if (byte_ptr == (uint8_t *)str)
			return (NULL);
		--byte_ptr;
	}
	if (*byte_ptr == c)
		return ((char *)byte_ptr);
	long_ptr = (uint64_t *)byte_ptr;
	while ((uint8_t *)long_ptr - (uint8_t *)str > 8 && \
		!mrt_has_zero(*(long_ptr - 1), c))
		--long_ptr;
	byte_ptr = (uint8_t *)long_ptr;
	while (*byte_ptr != c)
	{
		if (byte_ptr == (uint8_t *)str)
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
