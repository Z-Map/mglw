# ---------------------------------------------------------------------------- #
#
# Mglw tests
#
# ---------------------------------------------------------------------------- #

MGLW_TESTS_DIR		= test
MGLW_TESTS_SRCDIR	= $(MGLW_TESTS_DIR)/src
MGLW_TESTS			= movesquare font

MGLW_SIMPLE_TEST	= $(shell for atest in $(MGLW_TESTS); do if [ -e $(MGLW_TESTS_SRCDIR)/$$atest.c ]; then echo "$(TARGETDIR)/$$atest"; fi; done)
MGLW_HEAVY_TEST		= $(shell for atest in $(MGLW_TESTS); do if [ -d $(MGLW_TESTS_SRCDIR)/$$atest ]; then echo "$(TARGETDIR)/$$atest"; fi; done)

INT_TEST_TARGETS	= $(addprefix $(TARGETDIR)/,$(MGLW_TESTS))

.PHONY:	test $(INT_TEST_TARGETS)

test: $(TARGETDIR)/$(NAME) $(INT_TEST_TARGETS)

$(MGLW_SIMPLE_TEST): $(TARGETDIR)/$(NAME)
	@printf "\e[33mCompile $(notdir $@)\e[31m\e[80D"
	$(SILENT)$(CC) $(INT_CFLAG) -o $@ $(MGLW_TESTS_SRCDIR)/$(@:$(TARGETDIR)/%=%).c -L$(TARGETDIR) -lmglw $(INT_STATIC_LF)
	@printf "\e[80D%-79.79b \e[m[\e[32mok\e[m]\n" "\e[35m$(notdir $@)\e[m compiled !\e(B\e[m"

$(MGLW_HEAVY_TEST): $(TARGETDIR)/$(NAME)
	@echo "$@"
