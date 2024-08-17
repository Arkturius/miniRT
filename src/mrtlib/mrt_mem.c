// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_mem.c                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/16 20:59:14 by rgramati          #+#    #+#             //
//   Updated: 2024/08/16 21:34:37 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <unistd.h>

#include <mrtlib.h>

void	*mrt_memcpy(void *dst, const void *src, size_t n)
{
	t_u8	*d8;
	t_u8	*s8;
	t_u64	*d64;
	t_u64	*s64;

	if (dst && src)
	{
		d8 = (t_u8 *) dst;
		s8 = (t_u8 *) src;
		while (((t_uptr)s8 & (sizeof *d64 - 1)) && n && n--)
			*d8++ = *s8++;
		d64 = (t_u64 *) d8;
		s64 = (t_u64 *) s8;
		while (n > sizeof *d64)
		{
			*d64++ = *s64++;
			n -= sizeof *d64;
		}
		d8 = (t_u8 *) d64;
		s8 = (t_u8 *) s64;
		while (n && n--)
			*d8++ = *s8++;
	}
	return (dst);
}

void	*mrt_bzero(void *dst, size_t n)
{
	t_u8	*d8;
	t_u64	*d64;

	if (dst)
	{
		d8 = (t_u8 *) dst;
		while (((t_uptr)d8 & (sizeof *d64 - 1)) && n && n--)
			*d8++ = 0;
		d64 = (t_u64 *) d8;
		while (n > sizeof *d64)
		{
			*d64++ = 0;
			n -= sizeof *d64;
		}
		d8 = (t_u8 *) d64;
		while (n && n--)
			*d8++ = 0;
	}
	return (dst);
}
