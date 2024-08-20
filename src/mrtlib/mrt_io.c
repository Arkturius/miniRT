/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:41:38 by rgramati          #+#    #+#             */
/*   Updated: 2024/08/20 17:33:10 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include <mrtlib.h>
#include <mrt/error.h>

#define MRT_FILL_BYTE 42


t_error	mrt_io_open_file(const char *filename, t_s32 *fd, t_open_mode mode)
{
	while (1)
	{
		*fd = -1;
		if (!filename || !*filename)
			break ;
		*fd = open(filename, mode, 0644);
		if (*fd == -1)
			break ;
		return (MRT_SUCCESS);
	}
	return (MRT_ERR_FILE);
}

t_error	mrt_io_write(t_s32 fd, t_u8 *data, t_u64 size)
{
	t_s32	bytes;

	while (1)
	{
		if (!data || !size)
			break ;
		bytes = write(fd, data, size);
		while (bytes & (sizeof(t_u64) - 1))
			bytes += write(fd, "*", 1);
		return (MRT_SUCCESS);
	}
	return (MRT_FAIL);
}

t_error	mrt_io_save_array(t_s32 fd, va_list stream, t_u32 nmemb, t_u32 size)
{
	t_u8	*addr;
	t_u64	bytes[2];
	t_u64	chunk;

	while (1)
	{
		addr = va_arg(stream, t_u8 *);
		if (!addr)
			break ;
		bytes[0] = nmemb * size;
		if (bytes[0] / nmemb != (t_u32) size)
			break ;
		chunk = (t_u64) nmemb << 32 | size;
		mrt_io_write(fd, (t_u8 *) &chunk, sizeof(t_u64));
		chunk = (t_u64) ((-1UL / 255) * MRT_FILL_BYTE);
		mrt_io_write(fd, (t_u8 *) &chunk, sizeof(t_u64));
		bytes[1] = 0;
		while (bytes[1] < nmemb)
		{
			mrt_io_write(fd, addr + (bytes[1] * size), size);
			bytes[1]++;
		}
		return (MRT_SUCCESS);
	}
	return (MRT_FAIL);
}

t_error	mrt_io_save_stream(t_s32 fd, const char *fmt, va_list stream)
{
	char	*str;
	t_u32	nmemb;
	t_u32	size;
	t_u32	*tmp;

	str = (char *)fmt;
	nmemb = 1;
	size = 1;
	while (*str)
	{
		tmp = NULL;
		if (*str == '@')
			tmp = &nmemb;
		else if (*str == '>')
			tmp = &size;
		else if (*str == ',')
			mrt_io_save_array(fd, stream, nmemb, size);
		if (tmp)
			*tmp = va_arg(stream, t_u32);
		++str;
	}
	return (MRT_SUCCESS);
}


t_error	mrt_io_load_stream(t_s32 fd, const char *fmt, va_list stream)
{
	(void) fd;
	(void) fmt;
	(void) stream;
	return (MRT_FAIL);
}

/**
* @brief	Save a formatted stream of data into the file "filename"
*
* @param	filename	outfile name.
* @param	fmt			formatted stream
*						@	->	means start of an array, argument holds the 
*								number of elements.
*						>	->	means a size for last array, argument holds
*								the size of an element.
*						,	->	end of an array, argument holds the adress
*								to read from.
* 
* @note		Data arrays will be stored with they size and member size in a
*			small chunk preceding them (size_t);
*			When starting a new array, all of the remaining space before 64
*			bit alignment will be filled with 0x42 bytes.
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

t_error	mrt_io_load(const char *filename, const char *fmt, ...)
{
	va_list	data;
	t_s32	save;

	while (1)
	{
		if (mrt_io_open_file(filename, &save, MRT_OPEN_READ))
			break ;
		va_start(data, fmt);
		if (mrt_io_load_stream(save, fmt, data))
			break ;
		va_end(data);
		return (MRT_SUCCESS);
	}
	va_end(data);
	return (MRT_FAIL);
}
