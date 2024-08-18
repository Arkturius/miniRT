// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_parse_pobj.c                                   :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/17 20:39:05 by rgramati          #+#    #+#             //
//   Updated: 2024/08/18 20:26:43 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

t_error	mrt_parse_pobj_camera(t_pheader **obj, char *str, char **remain)
{
	t_pobj_camera	*cam;

	while (1)
	{
		cam = (t_pobj_camera *) mrt_pobj_new(MRT_OBJ_CAMERA);
		if (!cam || mrt_parse_vec(&cam->viewpoint, str, &str))
			break ;
		if (mrt_parse_vec(&cam->orientation, str, &str))
			break ;
		if (mrt_parse_int(&cam->fov, str, &str))
			break ;
		*obj = (t_pheader *) cam;
		*remain = str;
		return (MRT_SUCCESS);
	}
	mrt_pobj_clean((t_pheader *) cam);
	return (MRT_FAIL);
}

t_error	mrt_parse_pobj_light(t_pheader **obj, char *str, char **remain, int a)
{
	t_pobj_light	*lig;

	while (1)
	{
		if (a)
			lig = (t_pobj_light *) mrt_pobj_new(MRT_OBJ_AMBIENT);
		else
			lig = (t_pobj_light *) mrt_pobj_new(MRT_OBJ_LIGHT);
		if (!lig)
			break ;
		if (!a && mrt_parse_vec(&lig->lightpoint, str, &str))
			break ;
		if (mrt_parse_float(&lig->ratio, str, &str))
			break ;
		if (mrt_parse_color(&lig->color, str, &str))
			break ;
		*remain = str;
		*obj = (t_pheader *) lig;
		return (MRT_SUCCESS);
	}
	mrt_pobj_clean((t_pheader *) lig);
	return (MRT_FAIL);
}

t_error	mrt_parse_pobj_sphere(t_pheader **obj, char *str, char **remain)
{
	t_pobj_sphere	*sph;

	while (1)
	{
		sph = (t_pobj_sphere *) mrt_pobj_new(MRT_OBJ_SPHERE);
		if (!sph || mrt_parse_vec(&sph->center, str, &str))
			break ;
		if (mrt_parse_float(&sph->diameter, str, &str))
			break ;
		if (mrt_parse_color(&sph->color, str, &str))
			break ;
		*remain = str;
		*obj = (t_pheader *) sph;
		return (MRT_SUCCESS);
	}
	mrt_pobj_clean((t_pheader *) sph);
	return (MRT_FAIL);
}

t_error	mrt_parse_pobj_plane(t_pheader **obj, char *str, char **remain)
{
	t_pobj_plane	*pla;

	while (1)
	{
		pla = (t_pobj_plane *) mrt_pobj_new(MRT_OBJ_PLANE);
		if (!pla || mrt_parse_vec(&pla->position, str, &str))
			break ;
		if (mrt_parse_vec(&pla->norm, str, &str))
			break ;
		if (mrt_parse_color(&pla->color, str, &str))
			break ;
		*remain = str;
		*obj = (t_pheader *) pla;
		return (MRT_SUCCESS);
	}
	mrt_pobj_clean((t_pheader *) pla);
	return (MRT_FAIL);
}

t_error	mrt_parse_pobj_cylinder(t_pheader **obj, char *str, char **remain)
{
	t_pobj_cylinder	*cyl;

	while (1)
	{
		cyl = (t_pobj_cylinder *) mrt_pobj_new(MRT_OBJ_CYLINDER);
		if (!cyl || mrt_parse_vec(&cyl->center, str, &str))
			break ;
		if (mrt_parse_vec(&cyl->norm, str, &str))
			break ;
		if (mrt_parse_float(&cyl->diameter, str, &str))
			break ;
		if (mrt_parse_float(&cyl->height, str, &str))
			break ;
		if (mrt_parse_color(&cyl->color, str, &str))
			break ;
		*remain = str;
		*obj = (t_pheader *) cyl;
		return (MRT_SUCCESS);
	}
	mrt_pobj_clean((t_pheader *) cyl);
	return (MRT_FAIL);
}
