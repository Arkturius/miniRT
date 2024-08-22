/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_pobjs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 22:27:44 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/18 20:58:09 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

//remove this function please
void	mrt_pobj_print_bin(t_pheader *head, t_u32 type)
{
	t_u32			size;
	t_u8			*ptr;
	t_u8			*tmp;
	static t_u32	obj_sizes[7] = {0,	\
		sizeof(t_pobj_ambient),			\
		sizeof(t_pobj_camera),			\
		sizeof(t_pobj_light),			\
		sizeof(t_pobj_sphere),			\
		sizeof(t_pobj_plane),			\
		sizeof(t_pobj_cylinder)};

	size = obj_sizes[head->obj_type];
	ptr = (t_u8 *)head;
	tmp = ptr;
	printf("t_pheader <%p> = \n{\n", head);
	printf("  -> NEXT = %16p\n", (void *)*(t_u64 *)tmp);
	printf("  -> TYPE = %16lx\n", *(t_u64 *)(tmp + sizeof(t_u64)));
	printf("  -> POBJ = \n  {");
	tmp += sizeof(t_pheader);
	while (tmp - ptr < size)
	{
		if (!((t_uptr)tmp & 7))
			printf("\n    ");
		if (type == 16)
			printf("0x%016lx", *(t_u64 *)tmp);
		if (type == 2)
		{
			t_u32	i = -1, j = -1;
			while (++i < 8)
			{
				j = -1;
				while (++j < 8)
					printf("%c", (*(tmp + i) & (1 << (7 - j))) ? '1' : '0');
				printf(" ");
			}
		}
		tmp += sizeof(t_u64);
	}
	printf("\n  }\n}\n\n");
}

t_pheader	*mrt_pobj_new(t_objtype type)
{
	t_u32			size;
	t_pheader		*obj;
	static t_u32	obj_sizes[7] = {0,	\
		sizeof(t_pobj_ambient),			\
		sizeof(t_pobj_camera),			\
		sizeof(t_pobj_light),			\
		sizeof(t_pobj_sphere),			\
		sizeof(t_pobj_plane),			\
		sizeof(t_pobj_cylinder)};

	size = obj_sizes[type];
	obj = malloc(size);
	if (obj)
		mrt_bzero(obj, size);
	if (obj)
		obj->obj_type = type;
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

t_objtype	mrt_parse_pobj_type(char *str, char **remain)
{
	const char	*ids[7] = {"A", "C", "L", "sp", "pl", "cy", NULL};
	t_u32		index;

	if (!*str)
		return (MRT_OBJ_NONE);
	index = 1;
	while (ids[index - 1])
	{
		if (index < 4 && *(ids[index - 1]) == *str)
			break ;
		else if (index > 3 && *(t_u16 *)ids[index - 1] == *(t_u16 *)str)
			break ;
		++index;
	}
	*remain = str + 1 + (index > 3);
	return ((t_objtype)index);
}

t_error	mrt_parse_pobj(t_pheader **obj, char *str, char **remain)
{
	t_objtype	type;
	static char	*formats[7] = {"",	\
		MRT_PFORMAT_AMBIENT,		\
		MRT_PFORMAT_CAMERA,			\
		MRT_PFORMAT_LIGHT,			\
		MRT_PFORMAT_SPHERE,			\
		MRT_PFORMAT_PLANE,			\
		MRT_PFORMAT_CYLINDER};

	if (!str || !remain)
		return (MRT_FAIL);
	type = mrt_parse_pobj_type(str, &str);
	if (type == MRT_OBJ_NONE)
		return (MRT_FAIL);
	*obj = mrt_pobj_new(type);
	mrt_parse_pobj_data(obj, formats[type], str, &str);
	*remain = str;
	return (MRT_SUCCESS);
}
