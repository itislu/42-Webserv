ifndef PRINT_MK
PRINT_MK		:=	1

include				$(shell git rev-parse --show-toplevel)/Makefiles/setup.mk


# ********************************* COLORS *********************************** #

STY_RES			:=	"\033[0m"
STY_BOL			:=	"\033[1m"
STY_ITA			:=	"\033[3m"
STY_UND			:=	"\033[4m"
STY_RED			:=	"\033[31m"
STY_GRE			:=	"\033[32m"
STY_YEL			:=	"\033[33m"
STY_BLU			:=	"\033[34m"
STY_MAG			:=	"\033[35m"
STY_CYA			:=	"\033[36m"
STY_WHI			:=	"\033[37m"
STY_GRA			:=	"\033[90m"
STY_WHI_BRI		:=	"\033[97m"
STY_BLA_BG		:=	"\033[40m"
STY_RED_BG		:=	"\033[41m"
STY_GRE_BG		:=	"\033[42m"
STY_YEL_BG		:=	"\033[43m"
STY_BLU_BG		:=	"\033[44m"
STY_MAG_BG		:=	"\033[45m"
STY_CYA_BG		:=	"\033[46m"
STY_WHI_BG		:=	"\033[47m"
STY_GRA_BG		:=	"\033[100m"
STY_WHI_BRI_BG	:=	"\033[107m"


# **************************** PRINT FUNCTIONS ******************************* #

# Print argument with no newline added, with optional prefix from $(MSG_PREFIX)
define PRINT
if [[ -n "$(shell printf '%b' "$(1)")" ]]; then \
	echo -e -n "$(1)" \
		| awk -v prefix="$(shell printf '%b' $(MSG_PREFIX))" '{printf "%s%s", (prefix ? prefix " " : ""), $$0}'; \
fi
endef

# Print argument with newline added, with optional prefix from $(MSG_PREFIX)
define PRINTLN
if [[ -n "$(shell printf '%b' "$(1)")" ]]; then \
	echo -e "$(1)" \
		| awk -v prefix="$(shell printf '%b' $(MSG_PREFIX))" '{print (prefix ? prefix " " : "") $$0}'; \
fi
endef


# **************************** CUSTOM MESSAGES ******************************* #

MSG_LINK		:=	"🔗"
MSG_ARCHIVE		:=	"🔒"
MSG_UNKNOWN		=	$(STY_ITA)"Unknown target: "$(STY_BOL)$(STY_RED)"$@"$(STY_RES)
MSG_TRY_HELP	=	$(STY_ITA)"Try '"$(STY_BOL)"make help"$(STY_RES)$(STY_ITA)"' for a list of available targets."$(STY_RES)


#	Adjustments to dynamic messages

ifneq (, $(MSG_PREFIX))
MSG_PREFIX		:=	$(STY_WHI)$(MSG_PREFIX)$(STY_RES)
endif

ifneq (, $(MSG_USAGE))
MSG_USAGE		:=	"\n"$(MSG_USAGE)
endif


########################## Top-level only messages #############################
ifeq (0, $(MAKELEVEL))

#	Make status messages

MSG_MAKE_INFO	:=	$(STY_ITA)$(STY_WHI)"Make version: $(MAKE_VERSION)"$(STY_RES)
MSG_COMP_INFO	=	$(STY_ITA)$(STY_WHI)"Compiler version: $(CXX_VERSION)"$(STY_RES)
MSG_HELP		:=	$(STY_ITA)$(STY_WHI)"Run 'make help' to see all available Makefile targets."$(STY_RES)
MSG_START		=	$(STY_ITA)$(if $(RECOMPILE),"Rebuilding","Building")" $(NAME) ... "$(STY_RES)
MSG_SUCCESS		?=	$(STY_BOL)$(STY_ITA)$(COLOR_MAKE)"DONE!"$(STY_RES)
MSG_NO_CHANGE	:=	$(STY_ITA)$(STY_WHI)"Everything up-to-date!"$(STY_RES)
MSG_FAILURE		:=	$(STY_BOL)$(STY_ITA)$(STY_RED)"BUILD FAILED!"$(STY_RES)


#	Build modes

MSG_CLEAR		:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_GRA)"~~~~~~~~~~~~~~~~~~~~~ CLEAR MODE ~~~~~~~~~~~~~~~~~~~~~"$(STY_RES)
ifneq (, $(filter clear,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
MSG_MODE		:=	$(MSG_MODE)$(if $(MSG_MODE),"\n")$(MSG_CLEAR)
endif

MSG_OPT			:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_GRE)"~~~~~~~~~~~~~~~~~~~ OPTIMIZATION MODE ~~~~~~~~~~~~~~~~~~~"$(STY_RES)
ifneq (, $(filter opt,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
MSG_MODE		:=	$(MSG_MODE)$(if $(MSG_MODE),"\n")$(MSG_OPT)
endif

MSG_RUN			:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_YEL)"~~~~~~~~~~~~~~~~~~~~~~~ RUN MODE ~~~~~~~~~~~~~~~~~~~~~~~~"$(STY_RES)
ifneq (, $(filter run,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
MSG_MODE		:=	$(MSG_MODE)$(if $(MSG_MODE),"\n")$(MSG_RUN)
endif

MSG_SAN			:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_RED)"~~~~~~~~~~~~~~~~~~~~ SANITIZER MODE ~~~~~~~~~~~~~~~~~~~~~"$(STY_RES)
ifneq (, $(filter san,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
MSG_MODE		:=	$(MSG_MODE)$(if $(MSG_MODE),"\n")$(MSG_SAN)
endif

MSG_TERM		:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_YEL)"~~~~~~~~~~~~~~~~~~~ NEW TERMINAL MODE ~~~~~~~~~~~~~~~~~~~"$(STY_RES)
ifneq (, $(filter term,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
MSG_MODE		:=	$(MSG_MODE)$(if $(MSG_MODE),"\n")$(MSG_TERM)
endif

MSG_VAL			:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_BLU)"~~~~~~~~~~~~~~~~~~~~~ VALGRIND MODE ~~~~~~~~~~~~~~~~~~~~~"$(STY_RES)
ifneq (, $(filter val,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
MSG_MODE		:=	$(MSG_MODE)$(if $(MSG_MODE),"\n")$(MSG_VAL)
endif

MSG_VALFD		:=	$(STY_BOL)$(STY_ITA)$(STY_UND)$(STY_CYA)"~~~~~~~~~~~~~~~~~~~ VALGRIND FD MODE ~~~~~~~~~~~~~~~~~~~~"$(STY_RES)
ifneq (, $(filter valfd,$(MAKECMDGOALS) $(subst $(COMMA),$(SPACE),$(MODE))))
MSG_MODE		:=	$(MSG_MODE)$(if $(MSG_MODE),"\n")$(MSG_VALFD)
endif


#	Clean messages

MSG_CLEAN		:=	$(STY_ITA)"Cleaning up build artifacts ... "$(STY_RES)
MSG_FCLEAN		:=	$(STY_ITA)"Cleaning up build artifacts and executable ... "$(STY_RES)
MSG_FFCLEAN		:=	$(STY_ITA)"Forcefully cleaning up build artifacts directory, executable and generated documentation ... "$(STY_RES)


endif
################################################################################


# *************************** MAKEFILE DEBUGGING ***************************** #

print-%			:
					$(call PRINTLN,($(origin $*)) $(if $(filter-out undefined,$(flavor $*)),\
						$* $(if $(filter simple,$(flavor $*)),:=,=) $(shell echo -n $($*))))


endif
