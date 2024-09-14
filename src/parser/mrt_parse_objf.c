/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mrt_parse_objf.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:18:38 by rgramati          #+#    #+#             */
/*   Updated: 2024/09/13 13:59:17 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef MRT_BONUS

# include <stdio.h>

# include <mrtlib.h>
# include <mrt/error.h>
# include <mrt/parser.h>

t_error
	mrt_parse_obj_file(t_scene *scene, uint8_t *ptr, char *str, char **remain)
{
	t_error	err;

	err = (t_error){MRT_SUCCESS, (void *)__func__};
	(void) scene;
	(void) ptr;
	(void) str;
	(void) remain;
	while (mrt_isspace((unsigned char)*str))
		str++;
	printf("parsing an obj file [%s]\n", str);
	while (!mrt_isspace((unsigned char)*str))
		str++;
	*remain = str;
	return (err);
}

#endif
