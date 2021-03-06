/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/22 04:05:38 by qloubier          #+#    #+#             */
/*   Updated: 2016/12/19 17:11:36 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mglw_intern.h"

/* Coucou, je suis une bonne grosse globale des familles comme on les aime*/
static const struct wintypecb
{
	int		(*opener)(mglwin *, int, int, const char *);
	void	(*drawer)(mglwin *);
	void	(*imagedraw)(mglwin *, mglimg *, int, int);
	void	(*clearer)(mglwin *);
	void	(*closer)(mglwin *);
} MGLWtcb[] = {
	(struct wintypecb){ NULL, NULL, NULL, NULL, NULL },
	(struct wintypecb){
		&MGLWopener_legacy,
		&MGLWdrawer_legacy,
		NULL,
		&MGLWclearer_legacy,
		&MGLWcloser_legacy,
	},
	(struct wintypecb){
		&MGLWopener_opengl3,
		&MGLWdrawer_opengl3,
		NULL,
		&MGLWclearer_opengl3,
		&MGLWcloser_opengl3,
	},
	(struct wintypecb){
		&MGLWopener_opengl4,
		&MGLWdrawer_opengl4,
		NULL,
		&MGLWclearer_opengl4,
		&MGLWcloser_opengl4,
	},
	(struct wintypecb){
		&MGLWopener_vulkan,
		&MGLWdrawer_vulkan,
		NULL,
		&MGLWclearer_vulkan,
		&MGLWcloser_vulkan,
	}
};

mglwin		*mglw_mkwin(mglwm mode, mglwf flags)
{
	mglwin			win;
	mglwindata		wdata;
	mglwin			*ret;

	if (!mode)
		return (NULL);
	wdata = (mglwindata){ .user = 1, .state = 0, .window = NULL,
		.flags = flags & MGLW_WINDATAFLAGS,
		.layer2D = NULL, .draw_vao = 0,
		.draw_buffers = {0, 0, 0, 0, 0, 0, 0, 0}, .draw_tex = {0, 0, 0, 0},
		.optime = 0.0,
		.kcb = {NULL, NULL, NULL}, .kcb_args = {NULL, NULL, NULL} };
	win = (mglwin){ .data = NULL, .next = NULL,
		.mode = mode, .flags = flags, .creatime = glfwGetTime()};
	if (!(ret = malloc(sizeof(mglwin))))
		return (NULL);
	if (!(win.data = malloc(sizeof(mglwindata))))
	{
		free(ret);
		return (NULL);
	}
	*(win.data) = wdata;
	*ret = win;
	MGLWaddwin(ret);
	return (ret);
}

mglwin		*mglw_openwin(mglwin *win, int x, int y, const char *title)
{
	const int	m = win->mode;

	if ((!win) || (win->data->state & 1) ||
		!(MGLWtcb[m].opener) || !MGLWtcb[m].opener(win, x, y, title))
			return (NULL);
	return (win);
}

mglwin		*mglw_initwin(mglwin *win, int x, int y)
{
	if ((win->flags & MGLW_2DLAYER) &&
		!(win->data->layer2D = mglw_mktexture(
		(uint)x, (uint)y, MGLW_RGBA, MGLWI_DYNAMIC)))
		win->data->flags &= ~MGLW_2DLAYER;
	win->data->optime = glfwGetTime();
	win->data->state |= 1;
	glfwSetWindowUserPointer(win->data->window, win->data);
	glfwSetKeyCallback(win->data->window, &mglwin_keyprocess);
	return (win);
}

int			mglwin_run(mglwin *win)
{
	const int	m = win->mode;

	if ((win) && (win->data->state & 1))
	{
		if (MGLWtcb[m].drawer)
			MGLWtcb[m].drawer(win);
		glfwSwapBuffers(win->data->window);
		glfwPollEvents();
		if (glfwWindowShouldClose(win->data->window))
		{
			mglw_closewin(win);
			return (0);
		}
		if ((win->flags | win->data->flags)  & MGLW_STOP)
			return (0);
		glfwMakeContextCurrent(win->data->window);
		if (MGLWtcb[m].clearer)
			MGLWtcb[m].clearer(win);
		return (1);
	}
	return (0);
}

mglwin		*mglwin_stop(mglwin *win)
{
	win->flags &= MGLW_STOP;
	return (win);
}

void		mglwin_draw(mglwin *win)
{
	const int	m = win->mode;

	if ((win) && (win->data->state & 1))
	{
		if (MGLWtcb[m].drawer)
			MGLWtcb[m].drawer(win);
		glfwSwapBuffers(win->data->window);
	}
}

mglwin			*mglw_draw_itow(mglwin *win, mglimg *img, int x, int y)
{
	const int	m = win->mode;

	if ((win) && (win->data->state & 1))
	{
		if (MGLWtcb[m].imagedraw)
			MGLWtcb[m].imagedraw(win, img, x, y);
	}
	return (win);
}

int			mglwin_is_running(mglwin *win)
{
	if ((win) && (win->data->state & 1) &&
		!glfwWindowShouldClose(win->data->window))
		return (1);
	return (0);
}

mglwin		*mglw_closewin(mglwin *win)
{
	const int	m = win->mode;

	if ((win) && (win->data->state & 1))
	{
		if (win->flags & MGLW_2DLAYER)
			mglw_rmimg((mglimg *)(win->data->layer2D));
		if (MGLWtcb[m].closer)
			MGLWtcb[m].closer(win);
		glfwDestroyWindow(win->data->window);
		win->data->state = 0;
	}
	return (win);
}

void		mglw_rmwin(mglwin *win)
{
	if (!win)
		return ;
	MGLWdelwin(win);
	win->data->user -= 1;
	if (!(win->data->user))
	{
		mglw_closewin(win);
		free(win->data);
	}
	free(win);
}
