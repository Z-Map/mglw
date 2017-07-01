/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 10:50:07 by qloubier          #+#    #+#             */
/*   Updated: 2017/07/01 12:32:48 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mglw_intern/window.h"
#include "mglw_intern/image.h"

mglwin		*mglw_minsize(mglwin *win, int w, int h)
{
	return (mglw_limits(win, w, h, win->data->max_w, win->data->max_h));
}
mglwin		*mglw_maxsize(mglwin *win, int w, int h)
{
	return (mglw_limits(win, win->data->min_w, win->data->min_h, w, h));
}

mglwin		*mglw_limits(mglwin *win, int mi_w, int mi_h, int ma_w, int ma_h)
{
	if (!win)
		return (NULL);
	if (mi_w < 1)
		mi_w = GLFW_DONT_CARE;
	if (mi_h < 1)
		mi_h = GLFW_DONT_CARE;
	if (ma_w < 1)
		ma_w = GLFW_DONT_CARE;
	if (ma_h < 1)
		ma_h = GLFW_DONT_CARE;
	win->data->min_w = mi_w;
	win->data->min_h = mi_h;
	win->data->max_w = ma_w;
	win->data->max_h = ma_h;
	if (win->data->state & 1)
		glfwSetWindowSizeLimits(win->data->window, mi_w, mi_h, ma_w, ma_h);
	return (win);
}
