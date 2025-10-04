ifndef SUBMAKE_MK
SUBMAKE_MK		:=	1

include				$(shell git rev-parse --show-toplevel)/Makefiles/setup.mk
include				$(MAKEFILES_DIR)/print.mk


# ******************************** SUBMAKE *********************************** #

MAKEFLAGS		+=	--output-sync=recurse
MAKECMDGOALS	?=	all

#	Find all subdirectories containing Makefiles
SUBDIRS			:=	$(patsubst $(PWD)/%,%,$(patsubst %/,%,$(dir $(shell find $(PWD) -mindepth 2 -type f -name Makefile))))

.PHONY			:	$(MAKECMDGOALS)
$(MAKECMDGOALS)	:	$(SUBDIRS)
					$(call PRINTLN,"──────────────────────────────────────────────────────────────")

#	Recursively call make in subdirectories
.PHONY			:	$(SUBDIRS)
$(SUBDIRS)		:
					$(call PRINTLN,"──────────────────────────────────────────────────────────────")
				-	MAKELEVEL=$$(( $(MAKELEVEL) - 1 )) MSG_PREFIX="[$@]" $(MAKE) --directory=$@ $(MAKECMDGOALS)


endif
