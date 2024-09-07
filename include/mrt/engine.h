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

# define MRT_H		900
# define MRT_W		1600

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

t_error	mrt_mlx_init(t_mlx *mlx);

void	mrt_mlx_clean(t_mlx mlx);

t_u32	mrt_mlx_hook_setup(t_scene *scene);

/* MLX HOOKS **************************************************************** */

t_s32	mrt_mlx_hook_keyup(t_s32 key, void *mlx);

t_s32	mrt_mlx_hook_keydown(t_s32 key, void *mlx);

t_s32	mrt_mlx_hook_mousedown(t_s32 key, void *mlx);

t_s32	mrt_mlx_hook_win(t_s32 key, void *mlx);

/* ************************************************************************** */

// TODO BVH struct, object struct header, object structs, AABB struct 

typedef struct s_mrt_aabb
{
	t_mrt_vec	min;
	t_mrt_vec	max;
}	t_AABB;


typedef struct s_mrt_material
{
	t_mrt_color	obj;
	t_mrt_color	emi;
	t_f32		obj_r;
	t_f32		emi_r;
}	t_material;

// OBJECT STRUCTS

# define MRT_OBJ_CAMERA_FOV			0

# define MRT_OBJ_LIGHT_BRIGHTNESS	0

# define MRT_OBJ_AMBIENT_BRIGHTNESS	0

# define MRT_OBJ_SPHERE_DIAMETER	0

# define MRT_OBJ_CYLINDER_DIAMETER	0
# define MRT_OBJ_CYLINDER_HEIGHT	1

typedef struct s_mrt_object
{
	t_u32		type;
	t_u32		id;
	t_f32		data[2];
	t_mrt_vec	pos;
	t_mrt_vec	norm;
	t_material	mat;
}	t_object;

# define MRT_OBJ_CHUNK_CAP	63

typedef struct s_mrt_obj_chunk
{
	struct s_mrt_obj_chunk	*next;
	t_u32					capacity;
	t_u32					size;
	t_u8					padding[48];
	t_object				objs[MRT_OBJ_CHUNK_CAP];
}	t_obj_chunk;

t_obj_chunk	*mrt_obj_chunk_init(void);

void		mrt_obj_chunk_clean(t_obj_chunk *chunk);

t_object	*mrt_obj_alloc(t_obj_chunk *chunk, t_bool extend);

void		mrt_obj_free(t_obj_chunk *chunk);

// SCENE HANDLE

typedef struct s_mrt_scene
{
	t_mlx		mlx;
	t_u32		nobj;
	t_u32		nlig;
	t_obj_chunk	*objects;
	t_u8		*map;
	t_object	config[4];
}	t_scene;

typedef struct s_mrt_hit
{
	t_mrt_vec	point;
	t_mrt_color	color;
	t_bool		hit;
	t_f32		dist;
	t_object	*obj;
}	t_hit;

typedef struct s_mrt_ray
{
	t_mrt_vec	origin;
	t_mrt_vec	direction;
	t_hit		hit;
}	t_ray;

t_error		mrt_scene_init(t_file *file, t_scene *scene);

void		mrt_scene_clean(t_scene *scene);

int			mrt_scene_render(void *scene_ptr);

# define PARAM_A		0
# define PARAM_B		1
# define PARAM_C		2
# define PARAM_DELTA	3
# define PARAM_DELTA_R	4
# define PARAM_2_A		5

void		mrt_ray_init(t_scene *scene, t_ray *ray, t_u32 x, t_u32 y);

void		mrt_ray_cast(t_scene *scene, t_ray *ray);

void		mrt_ray_color(t_scene *scene, t_ray *ray);

void		mrt_sphere_inter(t_ray *ray, t_object *obj);

void		mrt_plane_inter(t_ray *ray, t_object *obj);

t_mrt_color	mrt_ray_color_ambient(t_scene *scene);

t_mrt_color	mrt_ray_color_diffuse(t_scene *scene, t_ray *ray);

#endif
