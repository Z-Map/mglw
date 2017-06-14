/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: map <map@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/18 21:59:43 by map               #+#    #+#             */
/*   Updated: 2017/06/08 22:38:51 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mglw_intern/window.h"
#include "mglw_intern/image.h"

mglwin		*mglw_setsizecb(mglwin *win, void (*f)(void *, int, int), void *arg)
{
	win->data->sizecb = f;
	win->data->sizecb_arg = arg;
	return (win);
}

mglwin		*mglw_setmcb(mglwin *win, int s, int (*f)(void *, double, double), void *arg)
{
	if (s)
	{
		win->data->mcb[1] = f;
		win->data->mcb_args[1] = arg;
	}
	else
	{
		win->data->mcb[0] = f;
		win->data->mcb_args[0] = arg;
	}
	return (win);
}

mglwin		*mglw_setkcb(mglwin *win, int s, int (*f)(void *, int), void *arg)
{
	if (s == GLFW_RELEASE)
	{
		win->data->kcb[0] = f;
		win->data->kcb_args[0] = arg;
	}
	if (s & GLFW_PRESS)
	{
		win->data->kcb[1] = f;
		win->data->kcb_args[1] = arg;
	}
	if (s & GLFW_REPEAT)
	{
		win->data->kcb[2] = f;
		win->data->kcb_args[2] = arg;
	}
	return (win);
}

void		MGLWsizeprocess(GLFWwindow *win, int w, int h)
{
	mglw_wd *const	wdata = (mglw_wd *)glfwGetWindowUserPointer(win);
	void			(*sizecb)(void *, int, int) = wdata->sizecb;
	GLFWwindow		*ctxsave;

	ctxsave = glfwGetCurrentContext();
	glfwMakeContextCurrent(win);
	wdata->screen_w = w;
	wdata->screen_h = h;
	if (!(wdata->flags & (MGLW_FULLRES | MGLW_FULLSCREEN)))
	{
		wdata->win_w = w;
		wdata->win_h = h;
	}
	glViewport(0, 0, w, h);
	if (wdata->flags & MGLW_2DLAYER)
		mglw_resizeimg((mglimg *)(wdata->layer2D), w, h, MGLW_TF_UNDEFINED);
	if (sizecb)
		sizecb(wdata->sizecb_arg, w, h);
	glfwMakeContextCurrent(ctxsave);
}

void		MGLWpositionprocess(GLFWwindow *win, int x, int y)
{
	mglw_wd *const	wdata = (mglw_wd *)glfwGetWindowUserPointer(win);

	if (!(wdata->flags & MGLW_FULLSCREEN))
	{
		wdata->win_x = x;
		wdata->win_y = y;
	}
}

void		MGLWmouseprocess(GLFWwindow* window, double xpos, double ypos)
{
	mglw_wd *const	wdata = (mglw_wd *)glfwGetWindowUserPointer(window);
	int				(*mcb)(void *, double, double) = wdata->mcb[0];

	if (mcb)
		mcb(wdata->mcb_args[0], xpos, ypos);
}

void		MGLWmousewheelprocess(GLFWwindow* window, double xpos, double ypos)
{
	mglw_wd *const	wdata = (mglw_wd *)glfwGetWindowUserPointer(window);
	int				(*mcb)(void *, double, double) = wdata->mcb[1];

	if (mcb)
		mcb(wdata->mcb_args[1], xpos, ypos);
}

void		MGLWmousebuttonprocess(GLFWwindow *win, int k, int s, int m)
{
	MGLWkeyprocess(win, k, -1, s, m);
}

void		MGLWkeyprocess(GLFWwindow *win, int k, int sc, int s, int m)
{
	mglw_wd *const	wdata = (mglw_wd *)glfwGetWindowUserPointer(win);
	int				(*kcb)(void *, int) = wdata->kcb[s];
	int				ret;

	(void)m;
	(void)sc;
	ret = -1;
	if (((k == mglw_getsetting(MGLWS_EXITKEY))
			&& (s & (GLFW_PRESS | GLFW_REPEAT)))
		|| ((kcb) && ((ret = kcb(wdata->kcb_args[s], k) < 0))))
		glfwSetWindowShouldClose(win, GLFW_TRUE);
}

mglwin		*mglwin_shouldclose(mglwin *win)
{
	glfwSetWindowShouldClose(win->data->window, GLFW_TRUE);
	return (win);
}
