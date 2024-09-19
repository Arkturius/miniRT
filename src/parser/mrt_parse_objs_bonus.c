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

#ifdef MRT_BONUS

# include <mrtlib.h>
# include <mrt/error.h>
# include <mrt/parser.h>
# include <mrt/engine.h>

static t_error
	mrt_parse_obj_type(t_objtype *type, char *str, char **remain)
{
	const char	*ids[8] = {"A", "C", "L", "sp", "pl", "cy", "ob", NULL};
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
	if (index < 8)
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
	static char	*fmts[8] = {" ", MRT_FMT_AMBIENT, MRT_FMT_CAMERA,
		MRT_FMT_LIGHT, MRT_FMT_SPHERE, MRT_FMT_PLANE, MRT_FMT_CYLINDER, NULL};

	err = mrt_parse_obj_type(&type, str, &str);
	if (err.type != MRT_SUCCESS)
		return (err);
	if (type < MRT_OBJ_LIGHT)
	{
		if (scene->config[type].type)
			return ((t_error){MRT_ERR_FMT_CONFIG, (void *)__func__});
		ptr = &scene->config[type];
	}
	else if (type == MRT_OBJ_LIGHT)
		ptr = mrt_obj_alloc(scene->lights, MRT_TRUE);
	else
		ptr = mrt_obj_alloc(scene->objects, MRT_TRUE);
	ptr->type = type;
	if (type == MRT_OBJ_OBJFILE)
		err = mrt_parse_obj_file(scene, (uint8_t *)ptr, str, &str);
	else
		err = mrt_parse_obj_format((uint8_t *)ptr, fmts[type], str, &str);
	*remain = str;
	return (err);
}

#endif
