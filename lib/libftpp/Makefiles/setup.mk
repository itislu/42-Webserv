ifndef SETUP_MK
SETUP_MK		:=	1


# ******************************* MAKE SETUP ********************************* #

MAKEFLAGS		+=	--silent --no-builtin-rules --no-builtin-variables
SHELL			:=	/bin/bash
PWD				:=	$(shell pwd)
MAKEFILES_DIR	:=	$(abspath $(dir $(lastword $(MAKEFILE_LIST))))
REPO_ROOT		:=	$(abspath $(dir $(MAKEFILES_DIR)))
REPO_SUBDIR		:=	$(patsubst /%,%,$(subst $(REPO_ROOT),,$(PWD)))


#	Special characters for Make

COMMA			:=	,
SPACE			:=	$() $()


# *************************** MAKEFILE DEBUGGING ***************************** #

print-%			:
					echo -e \($(origin $*)\) $(if $(filter-out undefined,$(flavor $*)),\
						\\t$* $(if $(filter simple,$(flavor $*)),:=,=) "$(shell echo -n $($*))")


endif
