/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrtlib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 21:00:20 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/25 16:59:51 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MRTLIB_H
# define MRTLIB_H

# include <unistd.h>
# include <stdint.h>
# include <fcntl.h>

typedef uint32_t		t_bool;

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
	float	x;
	float	y;
	float	z;
	float	w;
} __attribute__((aligned(16)))	t_mrt_vec;

typedef union u_mrt_color
{
	struct
	{
		uint8_t	b;
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;
	};
	uint32_t	argb;
}	t_mrt_color;

/* MRT_MEM ****************************************************************** */

void
*mrt_bzero(void *dst, size_t n);

void
*mrt_memcpy(void *dst, const void *src, size_t n);

size_t
mrt_strlen(const char *str);

char
*mrt_strdup(const char *str);

int
mrt_strcmp(const char *s1, const char *s2);

char
*mrt_strchr(const char *str, char c);

char
*mrt_strrchr(const char *str, char c);

int
mrt_isspace(int c);

int
mrt_isdigit(int c);

int
mrt_strtoi(char *str, char **remain, int *err);

float
mrt_strtof(char *str, char **remain, int *err);

typedef struct s_mrt_io_array
{
	uint32_t	nmemb;
	uint32_t	size;
	uint8_t		*addr;
}	t_io_array;

void
mrt_io_write(int32_t fd, uint8_t *data, uint64_t size);

void
mrt_io_read(int32_t fd, uint8_t *data, uint64_t size);

uint32_t
mrt_io_open_file(const char *filename, int32_t *fd, t_open_mode mode);

uint32_t
mrt_io_close_file(int32_t fd);

uint32_t
mrt_io_save(const char *filename, const char *fmt, ...);

uint32_t
mrt_io_load(const char *filename, const char *fmt, ...);

/* MRT_MATH ***************************************************************** */

float
mrt_fabs(float f);

uint32_t
mrt_min(uint32_t a, uint32_t b);

uint32_t
mrt_max(uint32_t a, uint32_t b);

uint32_t
mrt_clamp(uint32_t f, uint32_t cmin, uint32_t cmax);

/* MRT_VEC ****************************************************************** */

void
mrt_vec_add(t_mrt_vec v1, t_mrt_vec v2, t_mrt_vec *n);

void
mrt_vec_sub(t_mrt_vec v1, t_mrt_vec v2, t_mrt_vec *n);

void
mrt_vec_mult(t_mrt_vec v1, float f, t_mrt_vec *n);

float
mrt_vec_len(t_mrt_vec v);

void
mrt_vec_norm(t_mrt_vec v, t_mrt_vec *n);

float
mrt_vec_dot(t_mrt_vec v1, t_mrt_vec v2);

void
mrt_vec_cross(t_mrt_vec v1, t_mrt_vec v2, t_mrt_vec *n);

#endif // MRTLIB_H
