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

void	mrt_parse_pobj_step(t_u8 **ptr, char fmt, t_bool dir)
{
	t_u32	size;

	size = 0;
	if (fmt == 'i' || fmt == 'f' || fmt == 'c')
		size = sizeof(t_u32);
	if (fmt == 'v')
		size = sizeof(t_mrt_vec);
	if (dir)
		*ptr += size;
	else
		*ptr -= size;
}

t_error	mrt_parse_pobj_value(
	t_u8 **ptr,
	char fmt,
	char *str,
	char **remain
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

t_error	mrt_parse_pobj_data(
	t_pheader **obj,
	char *fmt,
	char *str,
	char **remain
)	{
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
		else if (*fmt == '>' && is_fmt)
			mrt_parse_pobj_step(&ptr, *(++fmt), MRT_TRUE);
		else if (*fmt == '<' && is_fmt)
			mrt_parse_pobj_step(&ptr, *(++fmt), MRT_FALSE);
		fmt++;
	}
	*remain = str;
	return (MRT_SUCCESS);
}
