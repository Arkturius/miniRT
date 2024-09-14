/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_mem.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 20:59:14 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/18 21:14:30 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include <mrtlib.h>

void	*mrt_memcpy(void *dst, const void *src, size_t n)
{
	uint8_t		*d8;
	uint8_t		*s8;
	uint64_t	*d64;
	uint64_t	*s64;

	if (dst && src)
	{
		d8 = (uint8_t *) dst;
		s8 = (uint8_t *) src;
		while (((uintptr_t)s8 & (sizeof(uint64_t) - 1)) && n && n--)
			*d8++ = *s8++;
		d64 = (uint64_t *) d8;
		s64 = (uint64_t *) s8;
		while (n >= sizeof(uint64_t))
		{
			*d64++ = *s64++;
			n -= sizeof(uint64_t);
		}
		d8 = (uint8_t *) d64;
		s8 = (uint8_t *) s64;
		while (n && n--)
			*d8++ = *s8++;
	}
	return (dst);
}

void	*mrt_bzero(void *dst, size_t n)
{
	uint8_t		*d8;
	uint64_t	*d64;

	if (dst)
	{
		d8 = (uint8_t *) dst;
		while (((uintptr_t)d8 & (sizeof(uint64_t) - 1)) && n && n--)
			*d8++ = 0;
		d64 = (uint64_t *) d8;
		while (n > sizeof(uint64_t))
		{
			*d64++ = 0;
			n -= sizeof(uint64_t);
		}
		d8 = (uint8_t *) d64;
		while (n && n--)
			*d8++ = 0;
	}
	return (dst);
}
