# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/03/03 18:39:00 by qloubier          #+#    #+#              #
#    Updated: 2017/07/01 12:32:16 by qloubier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= libmglw.a
LINKNAME		= mglw
PROJECTNAME		= mglw
PROJECTPATH		= $(CURDIR)
SILENT			= @
MGLWFLAGS		= -Wall -Wpadded -Wextra -Werror
CFLAGS			=
ARFLAGS			= -rcs
CDEFINES		=
OSXLIBS			= -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
LINUXLIBS		= -lrt -lm -ldl -lXrandr -lXinerama -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lGL

ifndef CC
  CC			= clang
endif

ifndef CXX
  CXX			= clang
endif

ifndef AR
  AR			= ar
endif

TARGETDIR		= .
LIBDIR			= lib
SRCDIR			= src
SRCDIR_SHADERS	= src/shaders
SRCDIR_GLFW		= $(LIBDIR)/glfw/src
SRCDIR_GLLOAD	= $(LIBDIR)/glload/source

INCDIR			= include $(SRCDIR)/include
INCDIR_SHADERS	= $(SRCDIR)/include/mgl/ressources/shaders
INCDIR_GLLOAD	= $(LIBDIR)/glload/include
INCDIR_GLFW		= $(LIBDIR)/glfw/include $(LIBDIR)/glfw/src

SRCS			=\
	system.c\
	system/settings.c\
	system/wlst.c\
	system/ilst.c\
	window.c\
	window/event.c\
	window/tools.c\
	window/layer.c\
	window/legacy.c\
	window/opengl3.c\
	window/opengl4.c\
	window/vulkan.c\
	monitor.c\
	image.c\
	texture.c\
	image_loader.c\
	error.c\
	mgl/shaders.c\
	mgl/strings/fromttf.c\
	mgl/strings/string.c\
	mgl/draw.c\

SHADERS			= pixelbox.vert pixelbox.frag\
	char.vert char.frag\
	rect.vert rect.frag\

SRCS_GLLOAD		= gl_load.c gl_load_cpp.cpp

SRCS_GLFW		=\
	context.c\
	init.c\
	input.c\
	monitor.c\
	vulkan.c\
	window.c

OPSYS=$(shell uname -s)

ifeq ($(OPSYS),Linux)
  CDEFINES		+= -D_GLFW_X11
  SRCS_GLFW		+=\
	x11_init.c\
	x11_monitor.c\
	x11_window.c\
	xkb_unicode.c\
	linux_joystick.c\
	posix_time.c\
	null_joystick.c\
	glx_context.c\
	egl_context.c\
	posix_thread.c\
	osmesa_context.c
  SRCS_GLLOAD	+= glx_load.c glx_load_cpp.cpp
  INT_STATIC_LF	= $(LINUXLIBS)
else
  CDEFINES		+= -D_GLFW_COCOA -D_GLFW_USE_CHDIR -D_GLFW_USE_RETINA\
	-D_GLFW_USE_MENUBAR
  SRCS_GLFW		+=\
	osmesa_context.c\
	posix_thread.c\
	egl_context.c\
	cocoa_time.c\
	cocoa_init.m\
	cocoa_joystick.m\
	cocoa_monitor.m\
	cocoa_window.m\
	nsgl_context.m
  LIB_CFLAGS	+= -Wno-deprecated-declarations -Wno-pointer-sign
  INT_STATIC_LF	= $(OSXLIBS)
endif

ifndef config
  config		= release
endif
ifeq ($(config),debug)
  CFLAGS		+= -O1 -g -fsanitize=address
endif
ifeq ($(config),release)
  CFLAGS		+= -Ofast
endif

BUILDDIR		= build/$(config)

# Intern vars
INT_BD			= $(PROJECTPATH)/$(BUILDDIR)

INT_INCFLAGS	= $(INCDIR:%=-I%)\
	$(INCDIR_SHADERS:%=-I%)\
	$(INCDIR_GLLOAD:%=-I%)\
	$(INCDIR_GLFW:%=-I%)

INT_LSRCS 		= $(SRCS_GLLOAD:%=$(SRCDIR_GLLOAD)/%)\
				$(SRCS_GLFW:%=$(SRCDIR_GLFW)/%)

INT_CSRCS		= $(filter %.c,$(INT_LSRCS))
INT_CXXSRCS		= $(filter %.cpp,$(INT_LSRCS))
INT_MSRCS		= $(filter %.m,$(INT_LSRCS))

INT_MGLWOBJ		= $(addprefix $(INT_BD)/, $(subst /,~,$(SRCS:%.c=mglw~%.o)))
INT_COBJ		= $(addprefix $(INT_BD)/, $(subst /,~,$(INT_CSRCS:%.c=%.o)))
INT_CXXOBJ		= $(addprefix $(INT_BD)/, $(subst /,~,$(INT_CXXSRCS:%.cpp=%.o)))
INT_MOBJ		= $(addprefix $(INT_BD)/, $(subst /,~,$(INT_MSRCS:%.m=%.o)))

INT_ALLOBJ		= $(INT_COBJ) $(INT_CXXOBJ) $(INT_MOBJ) $(INT_MGLWOBJ)

INT_CFLAG		= $(CFLAGS) $(MGLWFLAGS) $(INT_INCFLAGS)
INT_LCFLAG		= $(CFLAGS) $(LIB_CFLAGS) $(CDEFINES) $(INT_INCFLAGS)
INT_CXXFLAG		= $(CFLAGS) $(CDEFINES) $(INT_INCFLAGS)

INT_DEP			= $(INT_ALLOBJ:%.o=%.d)
INT_INCSHA		= $(SHADERS:%=$(INCDIR_SHADERS)/%.h)
INT_SHADERS		= $(SHADERS:%=$(INCDIR_SHADERS)/%.h)

BOBJ_GUARD		= $(shell if [ -d $(INT_BD) ]; then printf "on"; else printf "off"; fi)

.PHONY: all clean fclean re shaders static-libs $(INT_DEP)

all: $(INT_SHADERS) $(TARGETDIR)/$(NAME)

shaders:
	$(SILENT)$(MAKE) -Bs $(INT_SHADERS)

static-libs:
	@echo "$(INT_STATIC_LF)"

include test/tests.mk

$(INT_BD):
	$(SILENT)mkdir -p $(INT_BD)

$(INT_SHADERS): $(INCDIR_SHADERS)/%.h: $(SRCDIR_SHADERS)/%
	@printf "\e[33mShader $<\e[31m\e[80D"
	$(SILENT)printf "(const char[]){" > $@
ifeq ($(OPSYS),Linux)
	$(SILENT)xxd -i $< | grep -x "[0-9a-fx, ]*" | head -c -1 | tail -c +3 >> $@
else
	$(SILENT)xxd -i $< | grep -x "[0-9a-fx, ]*" >> $@
endif
	$(SILENT)printf " , 0x00}" >> $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$@\e[m\e(B\e[m\n"

$(INT_MGLWOBJ):
ifeq ($(BOBJ_GUARD),on)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CC) -MMD -MP $(INT_CFLAG) -o $@ -c $(subst ~,/,$(@:$(INT_BD)/mglw~%.o=$(SRCDIR)/%.c))
	@printf "\e[80D%-79.79b \e[m[\e[32mok\e[m]\n" "\e[35m$(notdir $@)\e[m compiled !\e(B\e[m"
endif

$(INT_COBJ):
ifeq ($(BOBJ_GUARD),on)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CC) -MMD -MP $(INT_LCFLAG) -o $@ -c $(subst ~,/,$(@:$(INT_BD)/%.o=%.c))
	@printf "\e[80D%-79.79b \e[m[\e[32mok\e[m]\n" "\e[35m$(notdir $@)\e[m compiled !\e(B\e[m"
endif

$(INT_CXXOBJ):
ifeq ($(BOBJ_GUARD),on)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CXX) -MMD -MP $(INT_LCFLAG) -o $@ -c $(subst ~,/,$(@:$(INT_BD)/%.o=%.cpp))
	@printf "\e[80D%-79.79b \e[m[\e[32mok\e[m]\n" "\e[35m$(notdir $@)\e[m compiled !\e(B\e[m"
endif

$(INT_MOBJ):
ifeq ($(BOBJ_GUARD),on)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CC) -MMD -MP $(INT_LCFLAG) -o $@ -c $(subst ~,/,$(@:$(INT_BD)/%.o=%.m))
	@printf "\e[80D%-79.79b \e[m[\e[32mok\e[m]\n" "\e[35m$(notdir $@)\e[m compiled !\e(B\e[m"
endif

-include $(INT_DEP)

ifeq ($(BOBJ_GUARD),off)
$(TARGETDIR)/$(NAME): $(INT_SHADERS) $(INT_BD)
	$(SILENT)$(MAKE) -s $@ BOBJ_GUARD=on
else
$(TARGETDIR)/$(NAME): $(INT_SHADERS) $(INT_ALLOBJ)
	$(SILENT)$(AR) $(ARFLAGS) $@ $(INT_ALLOBJ) $(INT_MGLWOBJ)
	@printf "\e[80D%-79.79b \e[m[\e[32mok\e[m]\n" "\e[35m$(NAME)\e[m compiled !\e(B\e[m"
endif

clean:
	@printf "\e[31mCleaning compile files ...\e(B\e[m\n"
	$(SILENT)rm -f $(INT_ALLOBJ) $(INT_DEP)

fclean: clean
	@printf "\e[31mCleaning $(NAME) ...\e(B\e[m\n"
	$(SILENT)rm -f $(TARGETDIR)/$(NAME)

re: fclean all
