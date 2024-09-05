// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_parse_objs.c                                   :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/09/04 18:14:02 by rgramati          #+#    #+#             //
//   Updated: 2024/09/05 21:13:35 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdarg.h>

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>
#include <mrt/engine.h>

static t_error
	mrt_parse_obj_value(t_u8 *ptr, char *fmt, char *str, char **remain)
{
	t_error	err;

	err = MRT_FAIL;
	if (*fmt == 'v')
		err = mrt_parse_vec((t_mrt_vec *)ptr, str, &str);
	if (*fmt == 'f')
		err = mrt_parse_float((t_f32 *)ptr, str, &str);
	if (*fmt == 'i')
		err = mrt_parse_int((t_s32 *)ptr, str, &str);
	if (*fmt == 'c')
		err = mrt_parse_color((t_mrt_color *)ptr, str, &str);
	if (err == MRT_SUCCESS)
		*remain = str;
	return (err);
}

static t_error
	mrt_parse_obj_format(t_u8 *ptr, char *fmt, char *str, char **remain)
{
	t_s32	off_err;
	t_u32	offset;
	t_error	err;

	if (!ptr)
		return (MRT_FAIL);
	err = MRT_SUCCESS;
	while (*fmt && fmt[1] && err == MRT_SUCCESS)
	{
		offset = mrt_strtoi(fmt, &fmt, &off_err);
		if (off_err)
			return (MRT_FAIL);
		while (*fmt && mrt_isspace((unsigned char)*fmt))
			++fmt;
		if (*fmt == '!' && mrt_strchr(MRT_FORMAT, *(fmt + 1)))
			err = mrt_parse_obj_value(ptr + offset, ++fmt, str, &str);
		++fmt;
	}
	*remain = str;
	return (err);
}

static t_error
	mrt_parse_obj_type(t_objtype *type, char *str, char **remain)
{
	const char	*ids[7] = {"A", "C", "L", "sp", "pl", "cy", NULL};
	t_u32		index;

	if (!*str)
		return (MRT_FAIL);
	index = 1;
	while (ids[index - 1])
	{
		if (index < 4 && *(ids[index - 1]) == *str)
			break ;
		else if (index > 3 && *(t_u16 *)ids[index - 1] == *(t_u16 *)str)
			break ;
		++index;
	}
	if (index == 7)
		return (MRT_FAIL);
	*remain = str + 1 + (index > 3);
	*type = (t_objtype)index;
	return (MRT_SUCCESS);
}

t_error
	mrt_parse_obj(t_scene *scene, char *str, char **remain)
{
	t_object	*ptr;
	t_objtype	type;
	t_error		err;
	static char	config[3];
	static char	*formats[7] = {" ", \
		MRT_FORMAT_AMBIENT, MRT_FORMAT_CAMERA, \
		MRT_FORMAT_LIGHT, MRT_FORMAT_SPHERE, \
		MRT_FORMAT_PLANE, MRT_FORMAT_CYLINDER};	

	if (mrt_parse_obj_type(&type, str, &str))
		return (MRT_ERR_FMT_TYPE);
	if (type < MRT_OBJ_SPHERE && config[type])
		return (MRT_ERR_FMT_CONFIG);
	else if (type < MRT_OBJ_SPHERE && !config[type])
	{
		config[type - 1] = 1;
		ptr = &scene->config[type];
	}
	else
		ptr = mrt_obj_alloc(scene->objects, MRT_TRUE);
	ptr->type = type;
	err = mrt_parse_obj_format((t_u8 *)ptr, formats[type], str, &str);
	*remain = str;
	return (err);
}
