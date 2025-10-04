ifndef BUILD_MK
BUILD_MK		:=	1

include				$(shell git rev-parse --show-toplevel)/Makefiles/setup.mk
include				$(MAKEFILES_DIR)/print.mk


# *************************** BUILD PREPARATION ****************************** #

export				MAKECMDGOALS


#	Build artifacts

OBJ				:=	$(SRC:$(SRC_DIR)/%=$(OBJ_DIR)/%.o)
DEP				:=	$(SRC:$(SRC_DIR)/%=$(DEP_DIR)/%.d)
OBJ_SUBDIRS		:=	$(sort $(dir $(OBJ)))
DEP_SUBDIRS		:=	$(sort $(dir $(DEP)))
.SECONDARY		:	$(OBJ) $(DEP) $(OBJ_SUBDIRS) $(DEP_SUBDIRS)


#	Target categories

BUILD_TARGETS	:=	all clear modes run term val valfd
REBUILD_TARGETS	:=	opt re san
DOC_TARGETS		:=	bear doxygen uml
CLEAN_TARGETS	:=	clean fclean ffclean
PHONY_TARGETS	:=	$(BUILD_TARGETS) $(REBUILD_TARGETS) $(DOC_TARGETS) $(CLEAN_TARGETS) libs
ENV_VARIABLES	:=	MODE ARGS TERMINAL
HELP_TARGETS	:=	help help-help help-print print-help \
					$(addprefix help-,$(PHONY_TARGETS) $(ENV_VARIABLES)) \
					$(addsuffix -help,$(PHONY_TARGETS) $(ENV_VARIABLES))
PRINT_TARGETS	:=	$(filter print-%,$(MAKECMDGOALS))
NO_DEP_TARGETS	:=	$(REBUILD_TARGETS) $(DOC_TARGETS) $(CLEAN_TARGETS) $(HELP_TARGETS) $(PRINT_TARGETS)
HIDDEN_TARGETS	:=	.bear-image .build .clang-uml .clang-uml-image .doxygen-image .plantuml .plantuml-image


#	Phony targets

PHONY_TARGETS	+=	$(HELP_TARGETS) $(HIDDEN_TARGETS) $(LIB_DIRS)
.PHONY			:	$(PHONY_TARGETS)


# ***************************** BUILD TARGETS ******************************** #

all				:
					if $(READY); then \
						$(call PRINTLN,"$(MSG_NO_CHANGE)"); \
						$(call PRINTLN,"$(MSG_HELP)"); \
					else \
						$(call PRINTLN,"$(MSG_MODE)"); \
						$(call PRINTLN," $(MSG_MAKE_INFO)"); \
						$(call PRINTLN," $(MSG_COMP_INFO)"); \
						$(call PRINTLN," $(MSG_HELP)"\n); \
						$(call PRINT,"$(MSG_START)"); \
						$(MAKE) .build; \
						result=$$?; \
						if [ $(MAKELEVEL) -eq 0 ]; then \
							echo; \
						fi; \
						if [ $$result -eq 0 ]; then \
							$(call PRINTLN,"$(MSG_SUCCESS)"); \
						else \
							$(call PRINTLN,"$(MSG_FAILURE)"); \
							exit 2; \
						fi; \
					fi; \
					$(call PRINTLN,"$(MSG_USAGE)")

clear opt run san term val valfd: modes

modes			:
					if [ "$(CLEAR)" = "true" ]; then \
						clear; \
					fi
					if [ "$(RECOMPILE)" = "true" ]; then \
						MAKELEVEL=$$(( $(MAKELEVEL) - 1 )) MSG_SUCCESS="" MSG_USAGE="" $(MAKE) re; \
						MAKELEVEL=$$(( $(MAKELEVEL) - 1 )) $(MAKE) clean; \
					else \
						MAKELEVEL=$$(( $(MAKELEVEL) - 1 )) MSG_USAGE="" $(MAKE) all; \
					fi
					if [ "$(CLEAR)" = "true" ]; then \
						clear; \
					fi
					if [ "$(NEW_TERM)" = "true" ] && [ -n "$(TERMINAL)" ]; then \
						$(TERMINAL) $(TERMINALFLAGS) bash -c \
							"unset MAKECMDGOALS; \
							trap ':' SIGINT; \
							$(ENV) ./$(NAME) $(value ARGS); \
							exec $(shell echo $$SHELL)"; \
					elif [ "$(RUN)" = "true" ]; then \
						$(ENV) "./$(NAME)" $(value ARGS); \
					else \
						$(call PRINTLN,"$(MSG_USAGE)"); \
					fi

re				:
					$(MAKE) fclean
					MAKELEVEL=$$(( $(MAKELEVEL) - 1 )) $(MAKE) all


# ***************************** BUILD PROCESS ******************************** #

#	Dependency files inclusion

#	Include if target is not known to not need dependency files
ifneq (, $(if $(MAKECMDGOALS),$(filter-out $(NO_DEP_TARGETS),$(MAKECMDGOALS) $(MODE)),all))
-include			$(DEP)
endif


#	Dependency management

define READY
$(NAME_READY) && $(LIBS_READY)
endef

define NAME_READY
$(MAKE) --question $(NAME) &>/dev/null
endef

define LIBS_READY
$(foreach lib,$(LIB_DIRS),$(MAKE) --question --directory=$(lib) &>/dev/null && ) true
endef

ifeq (4.4, $(firstword $(sort $(MAKE_VERSION) 4.4)))
.build			:	$(LIB_DIRS) .WAIT $(NAME)
else
.build			:	$(LIB_DIRS)
					$(MAKE) $(NAME)
endif

libs			:
					if ! $(LIBS_READY); then \
						$(MAKE) $(LIB_DIRS); \
						echo; \
					fi

$(LIB_DIRS)		:
					$(MAKE) --directory=$@


#	Linkage

ifneq (, $(IS_LIB))
#	Library
$(NAME)			:	$(OBJ)
					$(AR) $(ARFLAGS) $(NAME) $(OBJ) \
						&& echo -e -n $(MSG_ARCHIVE)
else
#	Executable
$(NAME)			:	$(LIBS_LOCAL) $(OBJ)
					$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(NAME) \
						&& echo -e -n $(MSG_LINK)
endif


#	Source file compilation

$(OBJ_DIR)/%.o	:	$(SRC_DIR)/% $(BUILDFILES) | $(OBJ_SUBDIRS)
					$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ \
						&& echo -e -n $(MSG_PROGRESS)


#	Pre-processing and dependency file creation

$(DEP_DIR)/%.d	:	$(SRC_DIR)/% $(BUILDFILES) | $(DEP_SUBDIRS)
					$(CXX) $(CPPFLAGS) $(DEPFLAGS) $(CXXFLAGS) $<


#	Directory structure mirroring of source files for build artifacts

$(OBJ_SUBDIRS) $(DEP_SUBDIRS):
					mkdir -p $@


# ********************************* MODES ************************************ #

MODE			:=	$(subst $(COMMA),$(SPACE),$(MODE))
ENV				:=

ifeq (clear, $(filter clear,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
CLEAR			:=	true
endif

ifeq (opt, $(filter opt,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
CXXFLAGS		:=	$(CXXFLAGS_STD) $(CXXFLAGS_OPT)
CPPFLAGS		+=	$(CPPFLAGS_OPT)
RECOMPILE		:=	true
endif

ifeq (re, $(filter re,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
RECOMPILE		:=	true
endif

ifeq (run, $(filter run,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
RUN				:=	true
endif

ifeq (san, $(filter san,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
CXXFLAGS		+=	$(CXXFLAGS_STD) $(CXXFLAGS_DBG) $(CXXFLAGS_SAN)
RECOMPILE		:=	true
endif

ifeq (term, $(filter term,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
NEW_TERM		:=	true
RUN				:=	true
endif

ifeq (val, $(filter val,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
ENV				+=	RUNNING_ON_VALGRIND=1 $(VALGRIND) $(VALGRINDFLAGS)
RUN				:=	true
endif

ifeq (valfd, $(filter valfd,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
ENV				+=	RUNNING_ON_VALGRIND=1 $(VALGRIND) $(VALGRINDFLAGS) $(VALGRINDFDFLAGS)
NEW_TERM		:=	true
RUN				:=	true
endif

ifdef ARGS
RUN				:=	true
endif


# ***************************** CLEAN TARGETS ******************************** #

clean			:
					$(call PRINTLN,"$(MSG_CLEAN)")
					for dir in $(LIB_DIRS); do \
						$(MAKE) clean --directory=$$dir; \
					done
					rm -f $(OBJ) $(DEP)
                    ifneq (, $(wildcard $(OBJ_DIR)))
					-find $(OBJ_DIR) -type d -empty -delete 2>/dev/null
                    endif
                    ifneq (, $(wildcard $(DEP_DIR)))
					-find $(DEP_DIR) -type d -empty -delete 2>/dev/null
                    endif
					$(call PRINTLN,"$(MSG_SUCCESS)")

fclean			:
					$(call PRINTLN,"$(MSG_FCLEAN)")
					$(MAKE) clean
					for dir in $(LIB_DIRS); do \
						$(MAKE) fclean --directory=$$dir; \
					done
					rm -f $(NAME)
					$(call PRINTLN,"$(MSG_SUCCESS)")

ffclean			:
					$(call PRINTLN,"$(MSG_FFCLEAN)")
					$(MAKE) fclean
					for dir in $(LIB_DIRS); do \
						$(MAKE) ffclean --directory=$$dir; \
					done
					rm -rf $(OBJ_DIR) $(DEP_DIR) $(DOXYGEN_OUTDIR) $(UML_OUTDIR)
					$(call PRINTLN,"$(MSG_SUCCESS)")


endif
