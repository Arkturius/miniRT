// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_triangles.c                                    :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/09/14 12:35:14 by rgramati          #+#    #+#             //
//   Updated: 2024/09/15 16:00:09 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/engine.h>

void
	mrt_triangle_get_points(t_mrt_vec **points, t_pbuffer *p, t_triangle *tri)
{
	uint32_t	i;
	uint32_t	pindex;

	i = 0;
	while (i < 3)
	{
		pindex = tri->indices[i];
		while (p && pindex > MRT_POINT_BUF_CAP)
		{
			pindex -= MRT_POINT_BUF_CAP;
			p = p->next;
		}
		points[i] = (t_mrt_vec *)&p->data[pindex];
		i++;
	}
}

#define TRI_A		0
#define TRI_B		1
#define TRI_C		2

#define VEC_E1		0
#define VEC_E2		1
#define VEC_RAY_E2	2
#define VEC_S		3
#define VEC_S_E1	0

#define VAL_DELTA	0
#define VAL_INVD	1
#define VAL_U		2
#define VAL_V		3
#define VAL_DIST	4

#include <stdio.h>
void
	mrt_triangle_inter(t_ray *ray, t_object *obj, t_pbuffer *p, t_triangle *tri)
{
	t_mrt_vec	*points[3];
	t_mrt_vec	vec[4];
	float		vals[5];

	mrt_triangle_get_points((t_mrt_vec **)&points, p, tri);
	mrt_vec_sub(*points[TRI_B], *points[TRI_A], &vec[VEC_E1]);
	mrt_vec_sub(*points[TRI_C], *points[TRI_A], &vec[VEC_E2]);
	mrt_vec_cross(ray->direction, vec[VEC_E2], &vec[VEC_RAY_E2]);
	vals[VAL_DELTA] = mrt_vec_dot(vec[VEC_E1], vec[VEC_RAY_E2]);
	if (mrt_fabs(vals[0]) < MRT_EPS)
		return ;
	vals[VAL_INVD] = 1.f / vals[VAL_DELTA];
	mrt_vec_sub(ray->origin, *points[TRI_A], &vec[VEC_S]);
	vals[VAL_U] = vals[VAL_INVD] * mrt_vec_dot(vec[VEC_S], vec[VEC_RAY_E2]);
	if (vals[VAL_U] < 0 || vals[VAL_U] > 1)
		return ;
	mrt_vec_cross(vec[VEC_S], vec[VEC_E1], &vec[VEC_S_E1]);
	vals[VAL_V] = vals[VAL_INVD] * mrt_vec_dot(ray->direction, vec[VEC_S_E1]);
	if (vals[VAL_V] < 0 || vals[VAL_V] + vals[VAL_U] > 1)
		return ;
	vals[VAL_DIST] = vals[VAL_INVD] * mrt_vec_dot(vec[VEC_E2], vec[VEC_S_E1]);
	if (vals[VAL_DIST] > MRT_EPS && vals[VAL_DIST] < ray->hit.dist)
		mrt_ray_update(ray, obj, vals[VAL_DIST]);
}

#include <stdio.h>
void
	mrt_obj_inter(t_ray *ray, t_object *obj, t_pbuffer *p, t_tbuffer *t)
{
	uint32_t	i;
	uint32_t	end;

	i = (uint32_t)obj->data[0];
	end = i + obj->data[1];
	while (t && i > MRT_TRIANGLE_BUF_CAP)
	{
		i -= MRT_TRIANGLE_BUF_CAP;
		t = t->next;
	}
	if (!t || i >= t->size)
		return ;
	while (i < end)
	{
		mrt_triangle_inter(ray, obj, p, &t->data[i]);
		i++;
	}
}
