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

#include <mrtlib.h>
#include <mrt/error.h>

#define MRT_FILL_BYTE 42

t_u32	mrt_io_open_file(const char *filename, t_s32 *fd, t_open_mode mode)
{
	*fd = -1;
	if (!filename || !*filename)
		return (MRT_ERR_FILE_NONE);
	*fd = open(filename, mode, 0644);
	if (*fd == -1)
		return (MRT_ERR_FILE_PERM);
	return (MRT_SUCCESS);
}

t_u32	mrt_io_close_file(t_s32 fd)
{
	t_s32	err;

	if (fd <= 2)
		return (MRT_ERR_FILE_NONE);
	err = close(fd);
	if (err == -1)
		return (MRT_ERR_FILE_NONE);
	return (MRT_SUCCESS);
}

void	mrt_io_write(t_s32 fd, t_u8 *data, t_u64 size)
{
	t_s32	err;

	if (!data || !size)
		return ;
	err = write(fd, data, size);
	if (err == -1)
		return ;
}

void	mrt_io_read(t_s32 fd, t_u8 *data, t_u64 size)
{
	t_s32	err;

	if (!data || !size)
		return ;
	err = read(fd, data, size);
	if (err == -1)
		return ;
}
