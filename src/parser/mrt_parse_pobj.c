/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_parse_pobj.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 20:39:05 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/18 20:26:43 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>

static t_error	mrt_parse_pobj_value(
	t_u8 **ptr, char fmt, char *str, char **remain
)	{
	t_error	err;

	err = MRT_SUCCESS;
	if (fmt == 'i')
		err = mrt_parse_int((int *)*ptr, str, &str);
	if (fmt == 'f')
		err = mrt_parse_float((float *)*ptr, str, &str);
	if (fmt == 'c')
		err = mrt_parse_color((t_mrt_color *)*ptr, str, &str);
	if (fmt == 'v')
		err = mrt_parse_vec((t_mrt_vec *)*ptr, str, &str);
	if (!err)
	{
		if ((fmt == 'i' || fmt == 'f' || fmt == 'c'))
			*ptr += sizeof(t_u32);
		if (fmt == 'v')
			*ptr += sizeof(t_mrt_vec);
	}
	if (fmt == '#')
	{
		while ((t_uptr) * ptr & (sizeof(t_u64) - 1))
			(*ptr)++;
	}
	*remain = str;
	return (err);
}

static t_error	mrt_parse_pobj_data(
	t_pheader **obj, char *fmt, char *str, char **remain)
{
	t_u8	*ptr;
	char	*is_fmt;

	ptr = (t_u8 *)*obj + sizeof(t_pheader);
	if (!ptr)
		return (MRT_FAIL);
	while (*fmt && *(fmt + 1))
	{
		is_fmt = mrt_strchr(MRT_PFORMAT, *(fmt + 1));
		if (*fmt == '!' && is_fmt)
		{
			fmt++;
			if (mrt_parse_pobj_value(&ptr, *fmt, str, &str))
				return (MRT_FAIL);
		}
		fmt++;
	}
	*remain = str;
	return (MRT_SUCCESS);
}

static t_objtype	mrt_parse_pobj_type(
	char *str, char **remain)
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

static t_error	mrt_parse_pobj(
	t_pheader **obj, char *str, char **remain)
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

t_error	mrt_parse_pobj_all(
	t_file *file)
{
	char		*data;
	t_pheader	*tmp;

	tmp = NULL;
	data = file->data;
	while (*data)
	{
		if (mrt_parse_pobj(&tmp, data, &data))
			return (MRT_FAIL);
		mrt_pobj_push(&file->objs, tmp);
		while (mrt_isspace(*data || *data == '\n'))
			data++;
	}
	if (*data)
		return (MRT_FAIL);
	return (MRT_SUCCESS);
}
