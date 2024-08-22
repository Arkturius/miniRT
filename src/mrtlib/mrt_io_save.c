/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_io_save.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:40:37 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/21 19:42:27 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>

#include <mrtlib.h>
#include <mrt/error.h>

t_error	mrt_io_save_array(t_s32 fd, t_io_array arr, t_bool sized)
{
	t_u64	bytes[2];
	t_u64	chunk;

	while (1)
	{
		bytes[0] = arr.nmemb * arr.size;
		if (bytes[0] / arr.nmemb != arr.size)
			break ;
		if (sized)
			chunk = (t_u64) arr.nmemb << 32 | arr.size;
		if (sized)
			mrt_io_write(fd, (t_u8 *) &chunk, sizeof(t_u64));
		bytes[1] = 0;
		while (bytes[1] < arr.nmemb)
		{
			mrt_io_write(fd, arr.addr + (bytes[1] * arr.size), arr.size);
			bytes[1]++;
		}
		return (MRT_SUCCESS);
	}
	return (MRT_FAIL);
}

t_error	mrt_io_save_stream(t_s32 fd, const char *fmt, va_list stream)
{
	char		*str;
	t_io_array	arr;
	t_bool		sized;

	str = (char *)fmt;
	arr = (t_io_array){.addr = NULL, .size = 1, .nmemb = 1};
	sized = MRT_TRUE;
	while (*str)
	{
		if (*str == '@')
			arr.addr = va_arg(stream, t_u8 *);
		else if (*str == '>' && str != fmt && *(str - 1) == '@')
			sized = MRT_TRUE;
		else if (*str == ' ')
		{
			arr.nmemb = va_arg(stream, t_u32);
			arr.size = va_arg(stream, t_u32);
			if (arr.addr)
				mrt_io_save_array(fd, arr, sized);
			sized = MRT_FALSE;
			arr.addr = NULL;
		}
		++str;
	}
	return (MRT_SUCCESS);
}

/**
* @brief	Save multiples arrays of data into the file "filename"
*
* @param	filename	outfile name.
* @param	fmt			formatted stream
*						@	->	means adress of an array to save.
*						>	->	means the array is size-specified, will have a
*								size chunk.
*						' '	->	array separator.
* @param	...			a list of n times (addr, nmemb, size) for each array
*
* @note		Data arrays can have a size chunk, depending on the appearance
*			of the '>' character after a '@' character.
*			Skiping all non control characters in formatted stream.
*			Ignores '>' if its not after a '@',
* 
* @return	MRT error code based on how things went.
*/
t_error	mrt_io_save(const char *filename, const char *fmt, ...)
{
	va_list	data;
	t_s32	save;

	while (1)
	{
		if (mrt_io_open_file(filename, &save, MRT_OPEN_WRITE))
			break ;
		va_start(data, fmt);
		if (mrt_io_save_stream(save, fmt, data))
			break ;
		va_end(data);
		return (MRT_SUCCESS);
	}
	va_end(data);
	return (MRT_FAIL);
}
