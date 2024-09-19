/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:02:24 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/25 19:09:54 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

# include <mrtlib.h>
# include <mrt/parser.h>

# define MRT_H		118
# define MRT_W		213

# define MRT_PI		3.141592
# define MRT_EPS	1e-5

typedef struct s_mlx
{
	void	*app;
	void	*win;
	void	*img;
}	t_mlx;

typedef struct e_mrt_error	t_error;

typedef struct s_mrt_scene	t_scene;

/* MLX FUNCS **************************************************************** */

t_error
mrt_mlx_init(t_mlx *mlx);

void
mrt_mlx_clean(t_mlx mlx);

uint32_t
mrt_mlx_hook_setup(t_scene *scene);

/* MLX HOOKS **************************************************************** */

int
mrt_mlx_hook_keyup(int key, void *mlx);

int
mrt_mlx_hook_keydown(int key, void *mlx);

int
mrt_mlx_hook_mousedown(int key, void *mlx);

int
mrt_mlx_hook_win(int key, void *mlx);

int
mrt_mlx_hook_loop(void *scene_ptr);

/* ************************************************************************** */

typedef struct s_mrt_aabb
{
	t_mrt_vec	min;
	t_mrt_vec	max;
} __attribute__((aligned(32)))	t_AABB;

typedef struct s_mrt_material
{
	t_mrt_color	obj;
	t_mrt_color	emi;
	float		obj_r;
	float		emi_r;
} __attribute__((aligned(16)))	t_material;

# define MRT_OBJ_CAMERA_FOV			0
# define MRT_OBJ_LIGHT_BRIGHTNESS	0
# define MRT_OBJ_AMBIENT_BRIGHTNESS	0
# define MRT_OBJ_SPHERE_DIAMETER	0
# define MRT_OBJ_CYLINDER_DIAMETER	0
# define MRT_OBJ_CYLINDER_HEIGHT	1

typedef struct s_mrt_object
{
	uint32_t	type;
	uint32_t	id;
	float		data[2];
	t_mrt_vec	pos;
	t_mrt_vec	norm;
	t_material	mat;
}	t_object;

# ifdef MRT_BONUS

typedef t_mrt_vec			t_point;

#  define MRT_POINT_BUF_CAP	255

typedef struct s_point_buffer
{
	struct s_point_buffer	*next;
	uint32_t				size;
	uint32_t				capacity;
	t_point					data[MRT_POINT_BUF_CAP];
}	t_pbuffer;

typedef struct s_triangle
{
	uint32_t	indices[4];
} __attribute__((aligned(16)))	t_triangle;

#  define MRT_TRIANGLE_BUF_CAP	255

typedef struct s_triangle_buffer
{
	struct s_triangle_buffer	*next;
	uint32_t					size;
	uint32_t					capacity;
	t_triangle					data[MRT_TRIANGLE_BUF_CAP];
}	t_tbuffer;

# endif

# define MRT_OBJ_CHUNK_CAP	63

typedef struct s_mrt_obj_chunk
{
	struct s_mrt_obj_chunk	*next;
	uint32_t				capacity;
	uint32_t				size;
	uint8_t					padding[48];
	t_object				objs[MRT_OBJ_CHUNK_CAP];
}	t_obj_chunk;

t_obj_chunk
*mrt_obj_chunk_init(void);

void
mrt_obj_chunk_clean(t_obj_chunk *chunk);

t_object
*mrt_obj_alloc(t_obj_chunk *chunk, t_bool extend);

void
mrt_obj_free(t_obj_chunk *chunk);

# ifndef MRT_BONUS

typedef struct s_mrt_scene
{
	t_mlx		mlx;
	uint32_t	nobj;
	uint32_t	nlig;
	t_object	config[4];
	t_obj_chunk	*objects;
}	t_scene;

# else

typedef struct s_mrt_scene
{
	t_mlx		mlx;
	uint32_t	nobj;
	uint32_t	nlig;
	t_object	config[4];
	t_obj_chunk	*lights;
	t_obj_chunk	*objects;
	t_tbuffer	*triangles;
	t_pbuffer	*points;
}	t_scene;

# endif

typedef struct s_mrt_hit
{
	t_mrt_vec	point;
	t_mrt_color	color;
	float		dist;
	t_object	*obj;
} __attribute__((aligned(32)))	t_hit;

typedef struct s_mrt_ray
{
	t_mrt_vec	origin;
	t_mrt_vec	direction;
	t_hit		hit;		
}	t_ray;

t_error
mrt_scene_init(t_file *file, t_scene *scene);

void
mrt_scene_clean(t_scene *scene);

int
mrt_scene_render(void *scene_ptr);

# define PARAM_A		0
# define PARAM_B		1
# define PARAM_C		2
# define PARAM_DELTA	3
# define PARAM_DELTA_R	4
# define PARAM_2_A		5

void
mrt_ray_init(t_scene *scene, t_ray *ray, uint32_t x, uint32_t y);

void
mrt_ray_cast(t_scene *scene, t_ray *ray);

void
mrt_ray_color(t_scene *scene, t_ray *ray);

void
mrt_ray_update(t_ray *ray, t_object *obj, float dist);

void
mrt_sphere_inter(t_ray *ray, t_object *obj);

void
mrt_plane_inter(t_ray *ray, t_object *obj);

# ifdef MRT_BONUS

void
mrt_obj_inter(t_ray *ray, t_object *obj, t_pbuffer *p, t_tbuffer *t);

# endif

t_mrt_color
mrt_ray_color_ambient(t_scene *scene);

t_mrt_color
mrt_ray_color_diffuse(t_scene *scene, t_ray *ray);

void
append_block(uint32_t bg, uint32_t fg, char *buf, char **remain);

int
mrt_scene_term_render(void *scene_ptr);

#endif
