CUR_PATH	    := $(shell pwd)
AXCL_HOME_PATH  ?= $(abspath $(dir $(CUR_PATH))..)

include $(CUR_PATH)/config.mak

COMPILE_TIME   := $(shell date +"%Y%m%d%H%M")
TARGET_PATH	   := $(AXCL_HOME_PATH)/out/$(NAME)_$(OS)_${HOST}

# Base dir
THIRD_PATH     := $(AXCL_HOME_PATH)/3rdparty
LOGGER_PATH    := $(AXCL_HOME_PATH)/logger
COMM_PATH      := $(AXCL_HOME_PATH)/comm
PROTOCOL_PATH  := $(AXCL_HOME_PATH)/protocol
HOST_PATH      := $(AXCL_HOME_PATH)/host
SAMPLE_PATH    := $(AXCL_HOME_PATH)/sample
TEST_PATH      := $(AXCL_HOME_PATH)/test
DRV_PATH       := $(AXCL_HOME_PATH)/drv
PYTHON_PATH    := $(AXCL_HOME_PATH)/python
FFMPEG_SRC     := $(THIRD_PATH)/ffmpeg

SUBDIRS        += $(THIRD_PATH) \
                  $(LOGGER_PATH) \
                  $(DRV_PATH) \
                  $(COMM_PATH)  \
                  $(PROTOCOL_PATH) \
                  $(HOST_PATH) \
                  $(FFMPEG_SRC) \
                  $(SAMPLE_PATH) \
                  $(TEST_PATH)

############################################################################
#	build
############################################################################
SUBDIRS_ALL     = $(addsuffix .all, $(SUBDIRS))
SUBDIRS_CLEAN   = $(addsuffix .clean, $(SUBDIRS))
SUBDIRS_INSTALL = $(addsuffix .install, $(SUBDIRS))

.EXPORT_ALL_VARIABLES:

.PHONY: $(SUBDIRS) $(SUBDIRS_ALL) $(SUBDIRS_INSTALL) $(SUBDIRS_CLEAN)
.NOTPARALLEL:

default: $(SUBDIRS)

prepare:
ifeq ($(HOST),x86)
#	@bash $(CUR_PATH)/check_compile_env.sh
endif

all: prepare $(SUBDIRS_ALL)
	@echo -e $(GREEN)"\nBuild All Modules success!!\n"  $(DONE)

install: $(SUBDIRS_INSTALL)
	@echo -e $(CYAN)"\nInstall success!!\n"  $(DONE)

ifeq ($(HOST),x86)
	@bash $(PYTHON_PATH)/build.sh
	@echo -e $(CYAN)"\nBuild python install package success!!\n"  $(DONE)
endif

clean:	$(SUBDIRS_CLEAN)
	@rm -rf $(TARGET_PATH)
	@echo -e $(GREEN)"\nClean success!!\n"  $(DONE)


$(SUBDIRS):
	@$(ECHO)
	@$(ECHO) -e $(CYAN)"In subdir $@ ..." $(DONE)
	@$(MAKE) -C $(basename $@ )

$(SUBDIRS_ALL):
	@$(ECHO) -e $(CYAN)"Making $(notdir $@)" $(DONE)
	@$(ECHO) -e $(CYAN)"In subdir $(basename $(notdir $@) )..." $(DONE)
	@$(MAKE) -C $(basename $@ ) all

$(SUBDIRS_INSTALL):
	@$(ECHO)
	@$(ECHO) -e $(CYAN)"In subdir $(basename $(notdir $@) )..." $(DONE)
	@$(MAKE) -C $(basename $@ ) install

$(SUBDIRS_CLEAN):
	@$(ECHO) -e $(CYAN)"In subdir $(basename $(notdir $@) )..." $(DONE)
	@$(MAKE) -C $(basename $@ ) clean
