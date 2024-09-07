/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_io_load.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:32:01 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/06 17:33:11 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_io_load.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:41:43 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/21 19:42:20 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>

#include <mrtlib.h>
#include <mrt/error.h>

t_u32	mrt_io_load_array(t_s32 fd, t_io_array arr, t_bool sized)
{
	t_u64	bytes[2];
	t_u64	chunk;

	if (!arr.addr)
		return (MRT_ERR_FILE_PROC);
	if (sized)
	{
		mrt_io_read(fd, (t_u8 *) &chunk, sizeof(t_u64));
		arr.nmemb = chunk >> 32;
		arr.size = chunk & 0xFFFFFFFF;
	}
	bytes[0] = arr.nmemb * arr.size;
	if (bytes[0] / arr.nmemb != arr.size)
		return (MRT_ERR_FILE_PROC);
	*(void **)(arr.addr) = malloc(bytes[0]);
	if (!*(void **)(arr.addr))
		return (MRT_ERR_ALLOC);
	bytes[1] = -1;
	while (++bytes[1] < arr.nmemb)
		mrt_io_read(fd, *(void **)(arr.addr) + (bytes[1] * arr.size), \
			arr.size);
	return (MRT_SUCCESS);
}

t_u32	mrt_io_load_stream(t_s32 fd, const char *fmt, va_list stream)
{
	char		*str;
	t_io_array	arr;
	t_bool		sized;
	t_error		err;

	str = (char *)fmt;
	arr = (t_io_array){.size = 1, .nmemb = 1, .addr = 0};
	sized = MRT_TRUE;
	while (*str)
	{
		if (*str == '&')
			arr.addr = (t_u8 *) va_arg(stream, void **);
		else if (*str == '<' && str != fmt && *(str - 1) == '&')
			sized = MRT_TRUE;
		else if (*str == ' ')
		{
			err.type = mrt_io_load_array(fd, arr, sized);
			if (err.type != MRT_SUCCESS)
				return (err.type);
			sized = MRT_FALSE;
		}
		++str;
	}
	return (MRT_SUCCESS);
}

/**
* @brief	Load "filename" into multiple arrays of data
*
* @param	filename	outfile name.
* @param	fmt			formatted stream
*						&	->	means an adress for storing.
*						<	->	means the array has size chunk.
*						' '	->	array separator.
* @param	...			all adresses for storing loaded data.
* 
* @note		Data adresses will be casted to (uint8_t *) for writing
*			Skiping all non control characters in formatted stream.
*			Ignores '<' if its not after a '&',
* 
* @return	MRT error code based on how things went.
*/
t_u32	mrt_io_load(const char *filename, const char *fmt, ...)
{
	va_list	data;
	t_s32	save;
	t_error	err;

	err.type = mrt_io_open_file(filename, &save, MRT_OPEN_READ);
	if (err.type != MRT_SUCCESS)
		return (err.type);
	va_start(data, fmt);
	err.type = mrt_io_load_stream(save, fmt, data);
	if (err.type != MRT_SUCCESS)
	{
		va_end(data);
		return (err.type);
	}
	va_end(data);
	return (MRT_SUCCESS);
}
