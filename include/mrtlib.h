// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrtlib.h                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/16 21:00:20 by rgramati          #+#    #+#             //
//   Updated: 2024/08/18 21:19:25 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef MRTLIB_H
# define MRTLIB_H

# include <unistd.h>
# include <stdint.h>

typedef uint64_t	t_u64;
typedef uint32_t	t_u32;
typedef uint16_t	t_u16;
typedef uint8_t		t_u8;

typedef int64_t		t_s64;
typedef int32_t		t_s32;
typedef int16_t		t_s16;
typedef int8_t		t_s8;

typedef uintptr_t	t_uptr;

typedef float		t_f32;
typedef double		t_f64;

typedef t_u32		t_bool;

# define MRT_TRUE		1
# define MRT_FALSE		0

# define MRT_FAIL		1
# define MRT_SUCCESS	0

typedef struct s_mrt_vec
{
	t_f32	x;
	t_f32	y;
	t_f32	z;
	t_f32	w;
}	t_mrt_vec;

typedef union u_mrt_color
{
	struct
	{
		t_u8	b;
		t_u8	g;
		t_u8	r;
		t_u8	a;
	};
	t_u32	argb;
}	t_mrt_color;

// MRT_MEM ****************************************************************** //

void	*mrt_bzero(void *dst, size_t n);

void	*mrt_memcpy(void *dst, const void *src, size_t n);

size_t	mrt_strlen(const char *str);

int		mrt_isspace(int c);

int		mrt_isdigit(int c);

int		mrt_strtoi(char *str, char **remain, int *err);

float	mrt_strtof(char *str, char **remain, int *err);

#endif // MRTLIB_H
