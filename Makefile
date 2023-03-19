# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/03/03 18:39:00 by qloubier          #+#    #+#              #
#    Updated: 2017/06/29 06:15:35 by qloubier         ###   ########.fr        #
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
LINUXLIBS		= -lrt -lm -lGL

LINUX_X11_LIBS	= -ldl -lXrandr -lXinerama -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

LINUX_WL_LIBS		= -lwayland-client
LINUX_WL_PROTODIR	= /usr/share/wayland-protocols
WL_XML_STABLE		= xdg-shell viewporter
WL_XML_UNSTABLE		= relative-pointer pointer-constraints idle-inhibit
WL_SPECIAL_CASE		= xdg-decoration

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

SHADERS			=\
	pixelbox.vert pixelbox.frag\
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

GENERATED_H	= mglw_gldef.h
GENERATED_C	=

OPSYS=$(shell uname -s)

ifeq ($(OPSYS),Linux)
ifeq ($(XDG_SESSION_TYPE),x11)
  CDEFINES		+= -D_GLFW_X11
  SRCS_GLFW		+=\
	glx_context.c\
	x11_init.c\
	x11_monitor.c\
	x11_window.c
  SRCS_GLLOAD	+= glx_load.c glx_load_cpp.cpp
  INT_STATIC_LF	= $(LINUX_X11_LIBS)
else
  CDEFINES		+= -D_GLFW_WAYLAND
  SRCS_GLFW		+=\
	wl_init.c\
	wl_monitor.c\
	wl_window.c
  INT_STATIC_LF	= $(LINUX_WL_LIBS)
  GENERATED_H	+=\
	$(WL_XML_STABLE:%=wayland-%-client-protocol.h)\
	$(WL_XML_STABLE:%=wayland-%-protocol.h)\
	$(WL_SPECIAL_CASE:%=wayland-%-client-protocol.h)\
	$(WL_SPECIAL_CASE:%=wayland-%-protocol.h)\
	$(WL_XML_UNSTABLE:%=wayland-%-unstable-v1-client-protocol.h)\
	$(WL_XML_UNSTABLE:%=wayland-%-unstable-v1-protocol.h)
  GENERATED_C	+=\
	$(WL_XML_STABLE:%=wayland-%-protocol.c)\
	$(WL_SPECIAL_CASE:%=wayland-%-protocol.c)\
	$(WL_XML_UNSTABLE:%=wayland-%-unstable-v1-protocol.c)
	
endif
  SRCS_GLFW		+=\
	xkb_unicode.c\
	linux_joystick.c\
	posix_time.c\
	null_joystick.c\
	egl_context.c\
	posix_thread.c\
	osmesa_context.c
  INT_STATIC_LF	+= $(LINUXLIBS)
else
  CDEFINES		+= -D_GLFW_COCOA -D_GLFW_USE_CHDIR -D_GLFW_USE_RETINA\
	-D_GLFW_USE_MENUBAR
  SRCS_GLFW		+=\
	posix_tls.c\
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
INT_GHD			= $(INT_BD)/generated/header
INT_GCD			= $(INT_BD)/generated/src

INT_INCFLAGS	= $(INCDIR:%=-I%)\
	$(INCDIR_SHADERS:%=-I%)\
	$(INT_GHD:%=-I%)\
	$(INCDIR_GLLOAD:%=-I%)\
	$(INCDIR_GLFW:%=-I%)

INT_GENH		= $(addprefix $(INT_GHD)/, $(GENERATED_H) $(SHADERS:%=%.h))

INT_GENC		= $(addprefix $(INT_GCD)/, $(GENERATED_C))

INT_LSRCS 		= $(SRCS_GLLOAD:%=$(SRCDIR_GLLOAD)/%)\
	$(SRCS_GLFW:%=$(SRCDIR_GLFW)/%)

INT_CSRCS		= $(filter %.c,$(INT_LSRCS))
INT_CXXSRCS		= $(filter %.cpp,$(INT_LSRCS))
INT_MSRCS		= $(filter %.m,$(INT_LSRCS))

INT_MGLWOBJ		= $(addprefix $(INT_BD)/, $(subst /,~,$(SRCS:%.c=mglw~%.o)))
INT_COBJ		= $(addprefix $(INT_BD)/, $(subst /,~,$(INT_CSRCS:%.c=%.o)))
INT_GENO		= $(INT_GENC:%.c=%.o)
INT_CXXOBJ		= $(addprefix $(INT_BD)/, $(subst /,~,$(INT_CXXSRCS:%.cpp=%.o)))
INT_MOBJ		= $(addprefix $(INT_BD)/, $(subst /,~,$(INT_MSRCS:%.m=%.o)))

INT_ALLOBJ		= $(INT_COBJ) $(INT_CXXOBJ) $(INT_MOBJ) $(INT_MGLWOBJ) $(INT_GENO)

INT_CFLAG		= $(CFLAGS) $(MGLWFLAGS) $(INT_INCFLAGS)
INT_LCFLAG		= $(CFLAGS) $(LIB_CFLAGS) $(CDEFINES) $(INT_INCFLAGS)
INT_CXXFLAG		= $(CFLAGS) $(CDEFINES) $(INT_INCFLAGS)

INT_DEP			= $(INT_ALLOBJ:%.o=%.d)
INT_INCSHA		= $(SHADERS:%=$(INCDIR_SHADERS)/%.h)
#INT_SHADERS		= $(SHADERS:%=$(INCDIR_SHADERS)/%.h)
INT_SHADERS		= $(SHADERS:%=$(INT_GHD)/%.h)

INT_NEED_DIR	= $(INT_BD)
ifneq ($(INT_GENH),)
  INT_NEED_DIR	+= $(INT_GHD)
endif
ifneq ($(INT_GENC),)
  INT_NEED_DIR	+= $(INT_GCD)
endif

#BOBJ_GUARD		= $(shell if [ -d $(INT_BD) ]; then printf "on"; else printf "off"; fi)
BOBJ_GUARD		= $(shell result="on"; for adir in $(INT_NEED_DIR); do if [ -d $$adir ]; then result="on"; else result="off"; break; fi; done; printf $$result)

.PHONY: all clean fclean re shaders static-libs generate-headers generate-src $(INT_DEP)

all: $(INT_SHADERS) $(TARGETDIR)/$(NAME)

shaders:
	$(SILENT)$(MAKE) -Bs $(INT_SHADERS)

static-libs:
	@echo "$(INT_STATIC_LF)"

include test/tests.mk

$(INT_BD) $(INT_GHD) $(INT_GCD):
	$(SILENT)mkdir -p $@

#$(INT_SHADERS): $(INCDIR_SHADERS)/%.h: $(SRCDIR_SHADERS)/%
#	@printf "\e[33mShader $<\e[31m\e[80D"
#	$(SILENT)printf "(const char[]){" > $@
#ifeq ($(OPSYS),Linux)
#	$(SILENT)xxd -i $< | grep -x "[0-9a-fx, ]*" | head -c -1 | tail -c +3 >> $@
#else
#	$(SILENT)xxd -i $< | grep -x "[0-9a-fx, ]*" >> $@
#endif
#	$(SILENT)printf " , 0x00}" >> $@
#	@printf "\e[m[\e[32mok\e[m] \e[35m$@\e[m\e(B\e[m\n"

$(INT_SHADERS): $(INT_GHD)/%.h: $(SRCDIR_SHADERS)/%
	@printf "\e[33mShader $(notdir $<)\e[31m\e[80D"
	$(SILENT)printf "(const char[]){" > $@
ifeq ($(OPSYS),Linux)
	$(SILENT)xxd -i $< | grep -x "[0-9a-fx, ]*" | head -c -1 | tail -c +3 >> $@
else
	$(SILENT)xxd -i $< | grep -x "[0-9a-fx, ]*" >> $@
endif
	$(SILENT)printf " , 0x00}" >> $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"

$(INT_GHD)/mglw_gldef.h: include/Makefile lib/glload/include/glload
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)$(MAKE) -s -C include TARGETDIR=$(INT_GHD)
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"

ifeq ($(OPSYS)-$(XDG_SESSION_TYPE),Linux-wayland)
$(WL_XML_STABLE:%=$(INT_GHD)/wayland-%-client-protocol.h): $(INT_GHD)/wayland-%-client-protocol.h: $(LINUX_WL_PROTODIR)/stable/%
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)wayland-scanner client-header $</$(notdir $<).xml $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"
$(WL_SPECIAL_CASE:%=$(INT_GHD)/wayland-%-client-protocol.h): $(INT_GHD)/wayland-%-client-protocol.h: $(LINUX_WL_PROTODIR)/unstable/%
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)wayland-scanner client-header $</$(notdir $<)-unstable-v1.xml $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"
$(WL_XML_UNSTABLE:%=$(INT_GHD)/wayland-%-unstable-v1-client-protocol.h): $(INT_GHD)/wayland-%-unstable-v1-client-protocol.h: $(LINUX_WL_PROTODIR)/unstable/%
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)wayland-scanner client-header $</$(notdir $<)-unstable-v1.xml $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"
$(WL_XML_STABLE:%=$(INT_GHD)/wayland-%-protocol.h): $(INT_GHD)/wayland-%-protocol.h: $(LINUX_WL_PROTODIR)/stable/%
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)wayland-scanner public-code $</$(notdir $<).xml $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"
$(WL_SPECIAL_CASE:%=$(INT_GHD)/wayland-%-protocol.h): $(INT_GHD)/wayland-%-protocol.h: $(LINUX_WL_PROTODIR)/unstable/%
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)wayland-scanner public-code $</$(notdir $<)-unstable-v1.xml $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"
$(WL_XML_UNSTABLE:%=$(INT_GHD)/wayland-%-unstable-v1-protocol.h): $(INT_GHD)/wayland-%-unstable-v1-protocol.h: $(LINUX_WL_PROTODIR)/unstable/%
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)wayland-scanner public-code $</$(notdir $<)-unstable-v1.xml $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"
$(WL_XML_STABLE:%=$(INT_GCD)/wayland-%-protocol.c): $(INT_GCD)/wayland-%-protocol.c: $(LINUX_WL_PROTODIR)/stable/%
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)wayland-scanner private-code $</$(notdir $<).xml $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"
$(WL_SPECIAL_CASE:%=$(INT_GCD)/wayland-%-protocol.c): $(INT_GCD)/wayland-%-protocol.c: $(LINUX_WL_PROTODIR)/unstable/%
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)wayland-scanner private-code $</$(notdir $<)-unstable-v1.xml $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"
$(WL_XML_UNSTABLE:%=$(INT_GCD)/wayland-%-unstable-v1-protocol.c): $(INT_GCD)/wayland-%-unstable-v1-protocol.c: $(LINUX_WL_PROTODIR)/unstable/%
	@printf "\e[33mGenerate $(notdir $@)\e[31m\e[90D"
	$(SILENT)wayland-scanner private-code $</$(notdir $<)-unstable-v1.xml $@
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m generated !\e(B\e[m\n"
endif

generate-headers: $(INT_GHD) $(INT_GENH)

generate-src: $(INT_GHD) $(INT_GCD) $(INT_GENH) $(INT_GENC)

$(INT_MGLWOBJ):
ifeq ($(BOBJ_GUARD),on)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CC) -MMD -MP $(INT_CFLAG) -o $@ -c $(subst ~,/,$(@:$(INT_BD)/mglw~%.o=$(SRCDIR)/%.c))
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m compiled !\e(B\e[m\n"
endif

$(INT_COBJ):
ifeq ($(BOBJ_GUARD),on)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CC) -MMD -MP $(INT_LCFLAG) -o $@ -c $(subst ~,/,$(@:$(INT_BD)/%.o=%.c))
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m compiled !\e(B\e[m\n"
endif

$(INT_GENO): %.o: %.c
ifeq ($(BOBJ_GUARD),on)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CC) -MMD -MP $(INT_LCFLAG) -o $@ -c $<
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m compiled !\e(B\e[m\n"
endif


$(INT_CXXOBJ):
ifeq ($(BOBJ_GUARD),on)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CXX) -MMD -MP $(INT_LCFLAG) -o $@ -c $(subst ~,/,$(@:$(INT_BD)/%.o=%.cpp))
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m compiled !\e(B\e[m\n"
endif

$(INT_MOBJ):
ifeq ($(BOBJ_GUARD),on)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CC) -MMD -MP $(INT_LCFLAG) -o $@ -c $(subst ~,/,$(@:$(INT_BD)/%.o=%.m))
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m compiled !\e(B\e[m\n"
endif

-include $(INT_DEP)

ifeq ($(BOBJ_GUARD),off)
$(TARGETDIR)/$(NAME): $(INT_SHADERS) $(INT_GHD) $(INT_GCD) $(INT_BD)
	$(SILENT)$(MAKE) -s $@ BOBJ_GUARD=on
else
$(TARGETDIR)/$(NAME): $(INT_SHADERS) $(INT_GENH) $(INT_ALLOBJ)
	$(SILENT)$(AR) $(ARFLAGS) $@ $(INT_ALLOBJ) $(INT_MGLWOBJ)
	@printf "\e[m[\e[32mok\e[m] \e[35m$(notdir $@)\e[m compiled !\e(B\e[m\n"
	@#printf "\e[80D%-79.79b \e[m[\e[32mok\e[m]\n" "\e[35m$(NAME)\e[m compiled !\e(B\e[m"
endif

clean:
	@printf "\e[31mCleaning compile files ...\e(B\e[m\n"
	$(SILENT)rm -f $(INT_ALLOBJ) $(INT_DEP) $(INT_GENH)

fclean: clean
	@printf "\e[31mCleaning $(NAME) ...\e(B\e[m\n"
	$(SILENT)rm -f $(TARGETDIR)/$(NAME)

re: fclean all
