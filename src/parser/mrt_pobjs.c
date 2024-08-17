// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_pobjs.c                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/14 22:27:44 by rgramati          #+#    #+#             //
//   Updated: 2024/08/16 21:39:42 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdlib.h>
#include <stdio.h>

#define MRTLIB_IMPLEMENTATION
#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

t_pheader	*mrt_pobj_new(t_pheader header)
{
	t_u32		size;
	t_pheader	*obj;

	size = sizeof * obj;
	if (header.next || !header.obj_type)
		return (NULL);
	else if (header.obj_type == MRT_OBJ_AMBIENT)
		size = sizeof(t_pobj_ambient);
	else if (header.obj_type == MRT_OBJ_CAMERA)
		size = sizeof(t_pobj_camera);
	else if (header.obj_type == MRT_OBJ_LIGHT)
		size = sizeof(t_pobj_light);
	else if (header.obj_type == MRT_OBJ_SPHERE)
		size = sizeof(t_pobj_sphere);
	else if (header.obj_type == MRT_OBJ_PLANE)
		size = sizeof(t_pobj_plane);
	else if (header.obj_type == MRT_OBJ_CYLINDER)
		size = sizeof(t_pobj_cylinder);
	obj = malloc(size);
	if (obj)
	{
		*obj = header;
		mrt_bzero(obj, size);
	}
	return (obj);
}

void	mrt_pobj_clean(t_pheader *list)
{
	t_pheader	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
}

static t_error	mrt_parse_line_join(char **dst, t_line *file, t_u32 size)
{
	t_line	*tmp;
	char	*join;

	if (!dst || !file)
		return (MRT_FAIL);
	tmp = file;
	while (tmp->next)
		tmp = tmp->next;
	size = size * MRT_LINE_LEN + mrt_strlen(tmp->str) + 1;
	join = malloc(size);
	if (!join)
		return (mrt_error_print(MRT_ERR_ALLOC, (void *)__func__));
	size = 0;
	tmp = file;
	while (tmp)
	{
		mrt_memcpy(join + size, tmp->str, MRT_LINE_LEN);
		tmp = tmp->next;
		size += MRT_LINE_LEN;
	}
	return (MRT_SUCCESS);
}

// convert each line of t_line into a t_pobj
static t_error	mrt_parse_pobj_convert(void)
{
	return (MRT_FAIL);
}

// full t_line to t_pobj conversion
t_error	mrt_parse_line_to_pobj(t_pheader **list, t_line *file)
{
	char	*line;

	(void) list;
	line = NULL;
	while (1)
	{
		if (mrt_parse_line_join(&line, file, mrt_line_count(file) - 1))
			break ;

		printf("HELLO THERE IS [%s] as a single string !\n", line);

		MRT_END_IMPLEMENTATION
		if (mrt_parse_pobj_convert())
			break ;
		mrt_line_clean(file);
		return (MRT_SUCCESS);
	}
	return (MRT_FAIL);
}
