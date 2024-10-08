/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_parse_objs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 18:14:02 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/07 14:51:28 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mrtlib.h>
#include <mrt/error.h>
#include <mrt/parser.h>
#include <mrt/engine.h>

t_errtype
	mrt_parse_obj_value(uint8_t *ptr, char *fmt, char *str, char **remain)
{
	t_errtype	err;

	err = MRT_SUCCESS;
	if (*fmt == 'v')
		err = mrt_parse_vec((t_mrt_vec *)ptr, str, &str);
	if (*fmt == 'f')
		err = mrt_parse_float((float *)ptr, str, &str);
	if (*fmt == 'i')
		err = mrt_parse_int((int32_t *)ptr, str, &str);
	if (*fmt == 'c')
		err = mrt_parse_color((t_mrt_color *)ptr, str, &str);
	if (err == MRT_SUCCESS)
	{
		*remain = str;
		return (err);
	}
	return (MRT_ERR_FMT_DATA);
}

t_error
	mrt_parse_obj_format(uint8_t *ptr, char *fmt, char *str, char **remain)
{
	int32_t		off_err;
	uint32_t	offset;
	t_error		err;

	if (!ptr)
		return ((t_error){MRT_ERR_FMT_DATA, (void *)__func__});
	err = (t_error){MRT_SUCCESS, (void *)__func__};
	while (*fmt && fmt[1] && err.type == MRT_SUCCESS)
	{
		offset = mrt_strtoi(fmt, &fmt, &off_err);
		if (off_err)
			return ((t_error){MRT_ERR_FMT_DATA, (void *)__func__});
		while (*fmt && mrt_isspace((unsigned char)*fmt))
			++fmt;
		if (*fmt == '!' && mrt_strchr(MRT_FORMAT, *(fmt + 1)))
			err.type = mrt_parse_obj_value(ptr + offset, ++fmt, str, &str);
		++fmt;
	}
	*remain = str;
	return (err);
}

#ifndef MRT_BONUS

static t_error
	mrt_parse_obj_type(t_objtype *type, char *str, char **remain)
{
	const char	*ids[7] = {"A", "C", "L", "sp", "pl", "cy", NULL};
	uint32_t	index;
	t_error		err;

	if (!*str)
		return ((t_error){MRT_ERR_FMT_TYPE, (void *)__func__});
	index = 1;
	err = (t_error){MRT_SUCCESS, (void *)__func__};
	while (ids[index - 1])
	{
		if (index < 4 && *(ids[index - 1]) == *str)
			break ;
		else if (index > 3 && *(uint16_t *)ids[index - 1] == *(uint16_t *)str)
			break ;
		++index;
	}
	if (index < 7)
	{
		*remain = str + 1 + (index > 3);
		*type = (t_objtype)index;
	}
	else
		err.type = MRT_ERR_FMT_TYPE;
	return (err);
}

t_error
	mrt_parse_obj(t_scene *scene, char *str, char **remain)
{
	t_object	*ptr;
	t_objtype	type;
	t_error		err;
	static char	config[3];
	static char	*fmts[7] = {" ", MRT_FMT_AMBIENT, MRT_FMT_CAMERA,
		MRT_FMT_LIGHT, MRT_FMT_SPHERE, MRT_FMT_PLANE, MRT_FMT_CYLINDER};

	err = mrt_parse_obj_type(&type, str, &str);
	if (err.type == MRT_SUCCESS)
	{
		if (type < MRT_OBJ_SPHERE)
		{
			if (config[type - 1])
				return ((t_error){MRT_ERR_FMT_CONFIG, (void *)__func__});
			config[type - 1] = 1;
			ptr = &scene->config[type];
		}
		else
			ptr = mrt_obj_alloc(scene->objects, MRT_TRUE);
		ptr->type = type;
		err = mrt_parse_obj_format((uint8_t *)ptr, fmts[type], str, &str);
		*remain = str;
	}
	return (err);
}

#endif
