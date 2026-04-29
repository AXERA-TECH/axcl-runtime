# Subdirectory list
SUBDIRS := main_app child_app

################################################################################
#	prepare param
################################################################################
CUR_PATH                  := $(shell pwd)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../../../)

include $(HOME_PATH)/build/config.mak

################################################################################
#	set task
################################################################################
SUBDIRS_CLEAN   = $(addsuffix .clean, $(SUBDIRS))
SUBDIRS_INSTALL = $(addsuffix .install, $(SUBDIRS))

.PHONY: $(SUBDIRS) $(SUBDIRS_INSTALL) $(SUBDIRS_CLEAN)
.NOTPARALLEL: clean install

# Default target
all: $(SUBDIRS)
	@$(ECHO) -e $(GREEN)"\nBuild All $(CURDIR) Modules success!!\n"  $(DONE)

# Build all subdirectories
$(SUBDIRS):
	@$(ECHO)
	@$(ECHO) -e $(CYAN)"In subdir $@ ..." $(DONE)
	@$(MAKE) -C $@

# Clean all subdirectories
clean:	$(SUBDIRS_CLEAN)
	@$(ECHO) -e $(GREEN)"\nClean $(CURDIR) success!!\n"  $(DONE)

# Install all subdirectories
install: $(SUBDIRS_INSTALL)
	@$(ECHO) -e $(GREEN)"\nInstall $(CURDIR) success!!\n"  $(DONE)

$(SUBDIRS_INSTALL):
	@$(ECHO)
	@$(ECHO) -e $(CYAN)"In subdir $(basename $@ )..." $(DONE)
	@$(MAKE) -C $(basename $@ ) install

$(SUBDIRS_CLEAN):
	@$(ECHO) -e $(CYAN)"In subdir $(basename $@ )..." $(DONE)
	@$(MAKE) -C $(basename $@ ) clean

# Export val
export MOD_TARGET_PATH
