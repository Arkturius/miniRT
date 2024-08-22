/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 20:43:35 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/18 21:30:54 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mrtlib.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include <mrt/error.h>
#include <mrt/parser.h>

static t_error	mrt_parse_line_join(char **dst, t_line *file, t_u32 size)
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
		return (mrt_error_print(MRT_ERR_ALLOC, (void *)__func__));
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

t_error	mrt_parse_line_to_pobj(t_pheader **list, t_line *file)
{
	static char	*line[2] = {NULL, NULL};
	t_error		ret;
	t_pheader	*tmp;

	tmp = NULL;
	ret = MRT_FAIL;
	while (1)
	{
		if (mrt_parse_line_join(&line[0], file, mrt_line_count(file) - 1))
			break ;
		line[1] = line[0];
		while (*line[0])
		{
			if (mrt_parse_pobj(&tmp, line[0], line))
				break ;
			mrt_pobj_push(list, tmp);
			while (mrt_isspace(*line[0]) || *line[0] == '\n')
				line[0]++;
		}
		ret = (t_error) !!*line[0];
		break ;
	}
	free(line[1]);
	return (ret);
}

static t_error	mrt_parse_line_list(t_line **list, t_parser *parser)
{
	char	buffer[MRT_LINE_LEN - 1];
	t_line	*file;
	t_line	*line;
	t_s32	bytes;

	file = NULL;
	line = NULL;
	while (1)
	{
		bytes = read(parser->fd, buffer, MRT_LINE_LEN - 1);
		buffer[bytes] = 0;
		if (bytes <= 0)
			break ;
		line = mrt_line_new((char *)&buffer, MRT_TRUE, bytes);
		if (!line)
			return (mrt_error_print(MRT_ERR_ALLOC, (void *)__func__));
		mrt_line_push(&file, line);
	}
	*list = file;
	return (MRT_SUCCESS);
}

static t_error	mrt_parse_file_lines(t_parser *parser)
{
	t_line		*file;
	t_pheader	*objs;

	file = NULL;
	objs = NULL;
	while (1)
	{
		if (mrt_parse_line_list(&file, parser))
			break ;
		if (mrt_parse_line_to_pobj(&objs, file))
			break ;
		mrt_line_clean(file);
		parser->objs = objs;
		while (objs)
		{
			++(parser->obj_count);
			mrt_pobj_print_bin(objs, 2);
			objs = objs->next;
		}
		if (!parser->objs)
			break ;
		return (MRT_SUCCESS);
	}
	mrt_line_clean(file);
	return (MRT_FAIL);
}

t_error	mrt_parse_file(const char *filename, t_parser *parser)
{
	char	*err;

	while (1)
	{
		err = "No filename provided...";
		if (!parser || !filename)
			break ;
		parser->filename = filename;
		err = "File couldn't be opened...";
		if (mrt_io_open_file(filename, &parser->fd, MRT_OPEN_READ))
			break ;
		err = "Errors found in file...";
		if (mrt_parse_file_lines(parser))
			break ;
		return (MRT_SUCCESS);
	}
	if (parser->fd != -1)
		close(parser->fd);
	return (mrt_error_print(MRT_ERR_FILE, (void *)err));
}
