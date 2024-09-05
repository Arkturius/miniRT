// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_scene_file.c                                   :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/09/05 20:43:00 by rgramati          #+#    #+#             //
//   Updated: 2024/09/05 21:04:35 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdio.h>
#include <stdlib.h>

#include <mrtlib.h>
#include <mrt/error.h>

char	*mrt_file_select(void)
{
	static char	filename[256];
	void		*res;
	FILE		*fp;

    fp = popen("zenity --file-selection --title=\"Select a file\"", "r");
	if (fp)
	{
		res = fgets(filename, sizeof(filename), fp);
		pclose(fp);
		if (!res)
			return (NULL);
		filename[mrt_strlen(filename) - !!mrt_strlen(filename)] = 0;
	}
	return (filename);
}
