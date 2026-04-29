CUR_PATH                  := $(shell pwd)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../../../../)

include $(HOME_PATH)/build/color.mk
include $(HOME_PATH)/build/config.mak

AXCL_HOME_PATH            := $(HOME_PATH)/axcl
AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib
SPDLOG_LIB_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/lib

BIN_PATH                  := $(CUR_PATH)/../../bin
PRJ_OUT_HOME              := $(HOME_PATH)/build/out/$(PROJECT)
OBJ_OUT_PATH              := $(PRJ_OUT_HOME)/objs
SRC_RELATIVE_PATH         := $(subst $(HOME_PATH)/,,$(CUR_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
ROOTFS_TARGET_PATH        := $(MSP_LIB_PATH)/..

# output
MOD_NAME                  := main_app
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCCPPS                   := main.cpp
SRCCPPS                   += $(wildcard $(CUR_PATH)/../../src/usrworker_ipc.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/../../src/usrworker_manager.cpp)

CINCLUDE                  := -I$(AXCL_HEADER_EXTERNAL_PATH)
CINCLUDE                  += -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/logger \
                             -I$(CUR_PATH)/../../src \
                             -I$(CUR_PATH)/../../src/ipc \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/include

OBJS                      := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)

# exec
TARGET                    := $(BIN_PATH)/$(MOD_NAME)

# build flags
CPPFLAGS                  := --std=c++17 -Wall
CFLAGS                    := $(CPPFLAGS)
CFLAGS                    += -DAPP_BUILD_VERSION=\"$(SDK_VERSION)\"

ifeq ($(debug),yes)
CFLAGS                    += -O0 -ggdb3
else
CFLAGS                    += -O2
endif

# dependency
CLIB                      := -lstdc++ -pthread -lrt
CLIB                      += -L$(MSP_LIB_PATH) -lax_logger
CLIB                      += -L$(SPDLOG_LIB_PATH) -lspdlog

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_DIR               := $(ROOTFS_TARGET_PATH)/bin/axclUsrWorker

# link - Use cross-compilation toolchain
LINK = $(CPP)

#.NOTPARALLEL: clean install
include $(HOME_PATH)/build/rules.mak