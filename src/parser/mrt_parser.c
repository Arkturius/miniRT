// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_parser.c                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/08/13 20:43:35 by rgramati          #+#    #+#             //
//   Updated: 2024/08/16 21:37:06 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <mrt/error.h>
#include <mrt/parser.h>

static t_error	mrt_parse_open(t_parser *parser)
{
	if (!parser)
		return (MRT_FAIL);
	parser->fd = open(parser->filename, O_RDONLY);
	if (parser->fd == -1)
		return (MRT_FAIL);
	return (MRT_SUCCESS);
}

static t_error	mrt_parse_file_lines(t_parser *parser)
{
	t_line		*file;
	t_pheader	*objs;

	file = NULL;
	while (1)
	{
		if (mrt_parse_line_list(&file, parser))
			break ;

		MRT_PRINT_LINES(file);

		if (mrt_parse_line_to_pobj(&objs, file))
			break ;	
		return (MRT_SUCCESS);
	}
	mrt_line_clean(file);
	return (MRT_FAIL);
}

t_error	mrt_parse_file(const char *filename, t_parser *parser)
{
	char	*err;

	parser->filename = filename;
	parser->fd = -1;
	while (1)
	{
		err = "No filename provided...";
		if (!filename)
			break ;
		err = "File not found...";
		if (mrt_parse_open(parser))
			break ;
		err = "Errors found in file...";
		if (mrt_parse_file_lines(parser))
			break ;
		return (MRT_SUCCESS);
	}
	return (mrt_error_print(MRT_ERR_FILE, (void *)err));
}
