# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/12/14 14:33:40 by qloubier          #+#    #+#              #
#    Updated: 2017/03/08 14:28:06 by qloubier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project Vars
NAME		= libglfw3.a
BUILDDIR	= .
TARGETDIR	= .
DEFINES		=
INCLUDES	= -Iinclude -Isrc
CFLAGS		=
CPPFLAGS	=
ARFLAG		= -rcs

OBJS	=$(BUILDDIR)/context.o\
		$(BUILDDIR)/init.o\
		$(BUILDDIR)/input.o\
		$(BUILDDIR)/monitor.o\
		$(BUILDDIR)/vulkan.o\
		$(BUILDDIR)/window.o
MOBJS	=
CXXOBJS	=

OPSYS=$(shell uname -s)
ifeq ($(OPSYS),Linux)
  DEFINES	+= -D_GLFW_X11
  OBJS		+= $(BUILDDIR)/x11_init.o\
			$(BUILDDIR)/x11_monitor.o\
			$(BUILDDIR)/x11_window.o\
			$(BUILDDIR)/xkb_unicode.o\
			$(BUILDDIR)/linux_joystick.o\
			$(BUILDDIR)/posix_time.o\
			$(BUILDDIR)/posix_tls.o\
			$(BUILDDIR)/glx_context.o\
			$(BUILDDIR)/egl_context.o
else
  DEFINES	+= -D_GLFW_COCOA -D_GLFW_USE_CHDIR -D_GLFW_USE_RETINA\
				-D_GLFW_USE_MENUBAR
  OBJS		+=$(BUILDDIR)/posix_tls.o\
			$(BUILDDIR)/cocoa_time.o
  MOBJS		+=$(BUILDDIR)/cocoa_init.o\
			$(BUILDDIR)/cocoa_joystick.o\
			$(BUILDDIR)/cocoa_monitor.o\
			$(BUILDDIR)/cocoa_window.o\
			$(BUILDDIR)/nsgl_context.o
  CFLAGS	+= -Wno-deprecated-declarations
endif

# Environnement Vars
ifndef config
  config = release
endif

ifndef verbose
  SILENT = @
endif

ifndef CC
  CC = gcc
endif

ifndef CXX
  CXX = g++
endif

ifndef AR
  AR = ar
endif

ifeq ($(config),debug)
  CFLAGS	+= -g
else
  CFLAGS	+= -O3 -fomit-frame-pointer -Wall
endif

CFLAGS		+= -MMD -MP $(DEFINES) $(INCLUDES)
CXXFLAGS	+= $(CFLAGS)
RESFLAGS	= $(DEFINES) $(INCLUDES)

ALLOBJS	= $(MOBJS) $(OBJS)
OAR		= $(ALLOBJS:%.o=%.d)

# Rules

.PHONY: clean fclean re objects

all: $(TARGETDIR)/$(NAME)

objects: $(ALLOBJS)

$(OBJS): $(BUILDDIR)/%.o: src/%.c
	@printf "\e[33mCompile $(notdir $<)\e[31m\e[80D"
	$(SILENT)$(CC) $(CFLAGS) -o "$@" -c "$<"
	@printf "\e[m[\e[32mok\e[m] \e[35m$@\e[m compiled !\e(B\e[m\n"

$(MOBJS): $(BUILDDIR)/%.o: src/%.m
	@printf "\e[33mCompile $(notdir $<)\e[31m\e[80D"
	$(SILENT)$(CC) $(CFLAGS) -o "$@" -c "$<"
	@printf "\e[m[\e[32mok\e[m] \e[35m$@\e[m compiled !\e(B\e[m\n"

-include $(OAR)

$(TARGETDIR)/$(NAME): $(ALLOBJS)
	@#echo Linking glfw
	$(SILENT)$(AR) $(ARFLAG) $@ $(OBJS) $(MOBJS)

clean:
	@echo Cleaning glfw
	$(SILENT) rm -rf $(ALLOBJS) $(OAR)

fclean: clean
	$(SILENT) rm -f  $(TARGETDIR)/$(NAME)

re: fclean all
