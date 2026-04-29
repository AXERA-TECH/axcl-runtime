CUR_PATH                  := $(shell pwd)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../../../../)

include $(HOME_PATH)/build/color.mk
include $(HOME_PATH)/build/config.mak

MSP_OUT_PATH              := $(HOME_PATH)/msp/out
MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib

AXCL_HOME_PATH            := $(HOME_PATH)/axcl
AXCL_OUT_PATH             := $(AXCL_HOME_PATH)/out
SPDLOG_LIB_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/lib
AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external

PRJ_OUT_HOME              := $(HOME_PATH)/build/out/$(PROJECT)
OBJ_OUT_PATH              := $(PRJ_OUT_HOME)/objs
SRC_RELATIVE_PATH         := $(subst $(HOME_PATH)/,,$(CUR_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
ROOTFS_TARGET_PATH        := $(MSP_OUT_PATH)

# output
MOD_NAME                  := usrworker_ut
OUTPUT                    := $(TARGET_OUT_PATH)/obj

# source
SRCCPPS                   := main.cpp

CINCLUDE                  := -I$(AXCL_HEADER_EXTERNAL_PATH)

OBJS                      := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)

# exec
TARGET                    := $(TARGET_OUT_PATH)/$(MOD_NAME)

# build flags
CPPFLAGS                  := --std=c++17 -Wall -Wl,-rpath-link=$(MSP_LIB_PATH)
CFLAGS                    := $(CPPFLAGS)
CFLAGS                    += -Werror -Wunused-function
CFLAGS                    += -DAPP_BUILD_VERSION=\"$(SDK_VERSION)\"

ifeq ($(debug),yes)
CFLAGS                    += -Wall -O0 -ggdb3
else
CFLAGS                    += -Wall -O2
endif

# dependency
CLIB                      := -lstdc++ -pthread
CLIB                      += -L$(MSP_LIB_PATH) -laxclrt_worker
CLIB                      += -L$(SPDLOG_LIB_PATH) -lspdlog

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_DIR               := $(AXCL_OUT_PATH)/usrworker

# link
LINK = $(CC)

#.NOTPARALLEL: clean install
include $(HOME_PATH)/build/rules.mak