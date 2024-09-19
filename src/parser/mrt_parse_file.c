/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_parse_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rems <rems@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:43:35 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/01 23:58:21 by rems             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mrtlib.h"
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include <mrt/error.h>
#include <mrt/parser.h>

static t_errtype	mrt_parse_line_join(char **dst, t_line *file, uint32_t size)
{
	t_line	*tmp;
	char	*join;

	if (!dst || !file)
		return (MRT_FAIL);
	tmp = file;
	while (tmp->next)
		tmp = tmp->next;
	size = size * (MRT_LINE_LEN - 1) + mrt_strlen(tmp->str) + 1;
	join = malloc(size);
	if (!join)
		return (MRT_ERR_ALLOC);
	mrt_bzero(join, size);
	size = 0;
	tmp = file;
	while (tmp)
	{
		mrt_memcpy(join + size, tmp->str, mrt_strlen(tmp->str));
		tmp = tmp->next;
		size += MRT_LINE_LEN - 1;
	}
	*dst = join;
	return (MRT_SUCCESS);
}

static t_errtype	mrt_parse_line_list(t_line **list, t_file *parser)
{
	char	buffer[MRT_LINE_LEN - 1];
	t_line	*line;
	int32_t	bytes;

	line = NULL;
	while (1)
	{
		bytes = read(parser->fd, buffer, MRT_LINE_LEN - 1);
		buffer[bytes] = 0;
		if (bytes <= 0)
			break ;
		line = mrt_line_new((char *)&buffer, MRT_TRUE, bytes);
		if (!line)
			return (MRT_ERR_ALLOC);
		mrt_line_push(list, line);
	}
	if (bytes < 0)
		return (MRT_FAIL);
	return (MRT_SUCCESS);
}

static t_errtype	mrt_parse_file_lines(t_file *parser)
{
	char		*data;
	t_line		*file;

	file = NULL;
	data = NULL;
	while (1)
	{
		if (mrt_parse_line_list(&file, parser))
			break ;
		if (mrt_parse_line_join(&data, file, mrt_line_count(file) - 1))
			break ;
		parser->data = data;
		mrt_line_clean(file);
		return (MRT_SUCCESS);
	}
	mrt_line_clean(file);
	return (MRT_FAIL);
}

/**
 * @todo	implement file extension recognition in mrt_io
 * 			support live scene opening
 * 			2nd parameter to check extension
 * 			support different type of files for miniRT
 */
static t_errtype	mrt_parse_file_extension(const char *filename, uint32_t *type)
{
	char		*point;

	*type = 0;
	while (1)
	{
		point = mrt_strrchr(filename, '.');
		if (!point)
			break ;
		if (!mrt_strcmp(point, ".rt"))
			*type = MRT_FILE_RT;
		if (!mrt_strcmp(point, ".cub"))
			*type = MRT_FILE_CUB;
		if (!mrt_strcmp(point, ".obj"))
			*type = MRT_FILE_OBJ;
		if (*type == MRT_FILE)
			break ;
		return (MRT_SUCCESS);
	}
	return (MRT_FAIL);
}

t_error	mrt_parse_file(const char *filename, t_file *parser)
{
	t_error	err;

	err = (t_error){MRT_ERR_FILE_NONE, (void *)__func__};
	while (1)
	{
		if (!parser || !filename)
			break ;
		parser->filename = filename;
		err.type = MRT_ERR_FILE_PERM;
		if (mrt_io_open_file(filename, &parser->fd, MRT_OPEN_READ))
			break ;
		err.type = MRT_ERR_FILE_EXTE;
		if (mrt_parse_file_extension(filename, &parser->type))
			break ;
		err.type = MRT_ERR_FILE_PROC;
		if (mrt_parse_file_lines(parser))
			break ;
		err.type = MRT_SUCCESS;
		return (err);
	}
	if (parser->fd != -1)
		close(parser->fd);
	return (err);
}
