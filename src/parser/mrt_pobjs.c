// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_pobjs.c                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/14 22:27:44 by rgramati          #+#    #+#             //
//   Updated: 2024/08/17 22:09:13 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdlib.h>
#include <stdio.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

t_pheader	*mrt_pobj_new(t_pheader pobj)
{
	t_u32		size;
	t_pheader	*obj;

	size = sizeof * obj;
	if (pobj.next || pobj.obj_type == MRT_OBJ_NONE)
		return (NULL);
	else if (pobj.obj_type == MRT_OBJ_AMBIENT)
		size = sizeof(t_pobj_ambient);
	else if (pobj.obj_type == MRT_OBJ_CAMERA)
		size = sizeof(t_pobj_camera);
	else if (pobj.obj_type == MRT_OBJ_LIGHT)
		size = sizeof(t_pobj_light);
	else if (pobj.obj_type == MRT_OBJ_SPHERE)
		size = sizeof(t_pobj_sphere);
	else if (pobj.obj_type == MRT_OBJ_PLANE)
		size = sizeof(t_pobj_plane);
	else if (pobj.obj_type == MRT_OBJ_CYLINDER)
		size = sizeof(t_pobj_cylinder);
	obj = malloc(size);
	if (obj)
	{
		*obj = pobj;
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

void	mrt_pobj_push(t_pheader **head, t_pheader *pobj)
{
	if (!head || !pobj)
		return ;
	pobj->next = *head;
	*head = pobj;
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
	*dst = join;
	return (MRT_SUCCESS);
}

t_objtype	mrt_parse_pobj_type(char *str, char **remain)
{
	const char	*ids[7] = {"A", "C", "L", "sp", "pl", "cy", NULL};
	char		**id;
	t_u16		t;

	if (!*str)
		return (MRT_OBJ_NONE);
	t = *(t_u16 *)str;
	id = (char **) ids;	
	while (id - (char **)ids < 3 && *id && **id != *str)
		id++;
	if (id - (char **)ids >= 3)
	{
		while (*id && *(t_u16 *)*id != t)
			id++;
	}
	if (!*id)
		return (MRT_OBJ_NONE);
	*remain = str + 1 + !(id - (char **)ids < 3);
	return ((t_objtype)(id - (char **)ids + 1));
}

static t_error	mrt_parse_pobj(t_pheader **obj, char *str, char **remain)
{
	t_objtype	type;

	if (!str || !remain)
		return (MRT_FAIL);
	type = mrt_parse_pobj_type(str, remain);
	if (type == MRT_OBJ_NONE)
		return (MRT_FAIL);
	if (type == MRT_OBJ_AMBIENT || type == MRT_OBJ_LIGHT)
		return (mrt_parse_pobj_light(obj, str, remain, type));
	if (type == MRT_OBJ_CAMERA)
		return (mrt_parse_pobj_camera(obj, str, remain));
	if (type == MRT_OBJ_SPHERE)
		return (mrt_parse_pobj_sphere(obj, str, remain));
	if (type == MRT_OBJ_PLANE)
		return (mrt_parse_pobj_plane(obj, str, remain));
	if (type == MRT_OBJ_CYLINDER)
		return (mrt_parse_pobj_cylinder(obj, str, remain));
	return (MRT_FAIL);
}

static t_error	mrt_parse_pobj_convert(t_pheader **list, char *file_line)
{
	t_pheader	*tmp;

	while (*file_line)
	{
		if (mrt_parse_pobj(&tmp, file_line, &file_line))
			break ;
		mrt_pobj_push(list, tmp);
	}
	if (*file_line)
		return (MRT_FAIL);
	return (MRT_SUCCESS);
}

t_error	mrt_parse_line_to_pobj(t_pheader **list, t_line *file)
{
	char	*line;

	(void) list;
	line = NULL;
	while (1)
	{
		if (mrt_parse_line_join(&line, file, mrt_line_count(file) - 1))
			break ;
		if (mrt_parse_pobj_convert(list, line))
			break ;


		MRT_END_IMPLEMENTATION
		return (MRT_SUCCESS);
	}
	return (MRT_FAIL);
}
