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

# define MRT_H	768
# define MRT_W	1367

typedef struct s_mlx
{
	void	*app;
	void	*win;
	void	*img;
}	t_mlx;

typedef enum e_mrt_error	t_error;

/* MLX FUNCS **************************************************************** */

t_error	mrt_mlx_init(t_mlx *mlx);

void	mrt_mlx_clean(t_mlx *mlx);

t_error	mrt_mlx_hook_setup(t_mlx *mlx);

/* MLX HOOKS **************************************************************** */

t_s32	mrt_mlx_hook_keyup(t_s32 key, void *mlx);

t_s32	mrt_mlx_hook_keydown(t_s32 key, void *mlx);

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

typedef struct s_mrt_obj_data
{
	t_material	mat;
	t_mrt_vec	pos;
	t_mrt_vec	norm;
	t_f32		data[4];
}	t_objD;

typedef struct s_mrt_object
{
	t_u32	type;
	t_u32	id;
	char	padding[24];
	t_AABB	aabb;
	t_objD	data;
}	__attribute__((aligned(128))) t_object;

# define MRT_OBJ_CHUNK_CAP	31

typedef struct s_mrt_obj_chunk
{
	struct s_mrt_obj_chunk	*next;
	t_u32					capacity;
	t_u32					size;
	t_object				objs[MRT_OBJ_CHUNK_CAP];
}	t_obj_chunk;

t_obj_chunk	*mrt_obj_chunk_init(void);

void		mrt_obj_chunk_clean(t_obj_chunk *chunk);

t_object	*mrt_obj_alloc(t_obj_chunk *chunk, t_bool extend);

void		mrt_obj_free(t_obj_chunk *chunk);

// SCENE HANDLE


typedef struct s_mrt_scene
{
	t_u32		nobj;
	t_u32		nlig;
	t_obj_chunk	*objects;
	// t_camera	cam;
}	t_scene;

t_error		mrt_parse_objs_all(t_scene *scene, t_file *file);

#endif
