/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_scene_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 20:43:00 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/07 14:48:11 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>

# include <mrtlib.h>
# include <mrt/error.h>
#include <unistd.h>

#define MRT_ZENITY_SAVE	1

char	*mrt_read_until(int fd)
{
	static char	buffer[256] = {0};
	int			total;
	int			bytes;

	total = 0;
	bytes = read(fd, buffer + total, 1);
	while (bytes > 0)
	{
		total += bytes;
		bytes = read(fd, buffer + total, 1);
		if (total + bytes >= 256)
		{
			buffer[0] = 0;
			break ;
		}
	}
	return (buffer);
}

void	*mrt_zenity_cleanup(int *fd, void *res)
{
	close(fd[0]);
	close(fd[1]);
	return (res);
}

char	*mrt_zenity(uint32_t mode, char **envp)
{
	char	*command[5] = {"/bin/zenity", "--file-selection", NULL, NULL, NULL};
	char	*filename;
	int		fd[2];
	int		pid;

	if (mode == MRT_ZENITY_SAVE)
		command[3] = "--save";
	if (pipe(fd))
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (mrt_zenity_cleanup(fd, NULL));
	if (!pid)
	{
		dup2(fd[1], STDOUT_FILENO);
		mrt_zenity_cleanup(fd, NULL);
		execve(command[0], command, envp);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	filename = mrt_read_until(fd[0]);
	return (mrt_zenity_cleanup(fd, filename));
}
