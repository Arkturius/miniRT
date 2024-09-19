// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   mrt_term_color.c                                   :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/09/16 17:06:20 by rgramati          #+#    #+#             //
//   Updated: 2024/09/17 13:16:36 by rgramati         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <stdint.h>
#include <unistd.h>

#define DBLOCK	"▄"
#define RGB_FG	"\033[38;2"
#define RGB_BG	"\033[48;2"
#define RESET	"\033[0m"

#define TERM_W	106
#define TERM_H	58

static void	str_cpy_fwd(char *dst, char *src, char **next_dst)
{
	if (!src || !dst)
		return ;
	while (*src)
		*dst++ = *src++;
	if (next_dst)
		*next_dst = dst;
}

static void	comp_to_str(uint8_t comp, char *buf)
{
	if (comp < 10)
		buf[0] = comp + '0';
	else if (comp < 100)
	{
		buf[0] = (comp / 10) + '0';
		buf[1] = (comp % 10) + '0';
	}
	else
	{
		buf[2] = (comp % 10) + '0';
		comp /= 10;
		buf[1] = (comp % 10) + '0';
		buf[0] = (comp / 10) + '0';
	}
}

static void	append_fg(uint32_t argb, char *buf, char **remain)
{
	char		color[8];
	uint8_t		*comp;
	uint32_t	i;

	str_cpy_fwd(buf, RGB_FG, &buf);
	i = 0;
	comp = ((uint8_t *)&argb) + 2;
	while (i < 3)
	{
		*(uint64_t *)color = 0;
		*color = ';';
		comp_to_str(*comp, &color[1]);
		str_cpy_fwd(buf, color, &buf);
		i++;
		comp--;
	}
	str_cpy_fwd(buf, "m", &buf);
	*remain = buf;
}

static void	append_bg(uint32_t argb, char *buf, char **remain)
{
	char		color[8];
	uint8_t		*comp;
	uint32_t	i;

	str_cpy_fwd(buf, RGB_BG, &buf);
	i = 0;
	comp = ((uint8_t *)&argb) + 2;
	while (i < 3)
	{
		*(uint64_t *)color = 0;
		*color = ';';
		comp_to_str(*comp, &color[1]);
		str_cpy_fwd(buf, color, &buf);
		i++;
		comp--;
	}
	str_cpy_fwd(buf, "m", &buf);
	*remain = buf;
}

void	append_block(uint32_t bg, uint32_t fg, char *buf, char **remain)
{
	append_bg(bg, buf, &buf);
	append_fg(fg, buf, &buf);
	str_cpy_fwd(buf, DBLOCK, &buf);
	*remain = buf;
}
