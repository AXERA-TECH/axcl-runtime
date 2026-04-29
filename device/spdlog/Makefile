CUR_PATH           := $(shell pwd)
HOME_PATH          := $(abspath $(CUR_PATH)/../../..)
ROOTFS_TARGET_PATH ?= $(HOME_PATH)/rootfs/rootfs/usr/local

include $(HOME_PATH)/build/color.mk
include $(HOME_PATH)/build/config.mak

SRC_LIB_PATH       := $(HOME_PATH)/axcl/3rdparty/spdlog/arm64/lib
INSTALL_PATH       := $(ROOTFS_TARGET_PATH)/lib

all:
	@$(ECHO) -e $(GREEN)"\nBuild spdlog success!!\n"  $(DONE)

install:
	@$(MKDIR) $(INSTALL_PATH)
	@$(CP) -d $(SRC_LIB_PATH)/libspdlog.so* $(INSTALL_PATH)
	@$(ECHO) -e $(GREEN)"\nInstall spdlog success!!\n"  $(DONE)

clean:
	@$(RM) $(INSTALL_PATH)/libspdlog.so*
	@$(ECHO) -e $(GREEN)"\nClean spdlog success!!\n"  $(DONE)