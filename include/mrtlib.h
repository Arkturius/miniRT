// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrtlib.h                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/16 21:00:20 by rgramati          #+#    #+#             //
//   Updated: 2024/08/22 23:17:18 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef MRTLIB_H
# define MRTLIB_H

# include <unistd.h>
# include <stdint.h>
# include <fcntl.h>

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

typedef enum e_open_mode
{
	MRT_OPEN_READ = O_RDONLY,
	MRT_OPEN_WRITE = O_WRONLY | O_CREAT | O_TRUNC,
}	t_open_mode;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

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

char	*mrt_strdup(const char *str);

char	*mrt_strchr(const char *str, char c);

int		mrt_isspace(int c);

int		mrt_isdigit(int c);

int		mrt_strtoi(char *str, char **remain, int *err);

float	mrt_strtof(char *str, char **remain, int *err);

typedef struct s_mrt_io_array
{
	t_u32	nmemb;
	t_u32	size;
	t_u8	*addr;
}	t_io_array;

t_u32	mrt_io_error(const char *func);

void	mrt_io_write(t_s32 fd, t_u8 *data, t_u64 size);

void	mrt_io_read(t_s32 fd, t_u8 *data, t_u64 size);

t_u32	mrt_io_open_file(const char *filename, t_s32 *fd, t_open_mode mode);

t_u32	mrt_io_close_file(t_s32 fd);

t_u32	mrt_io_save(const char *filename, const char *fmt, ...);

t_u32	mrt_io_load(const char *filename, const char *fmt, ...);

#endif // MRTLIB_H
