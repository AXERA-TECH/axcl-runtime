CUR_PATH                  := $(shell pwd)
COMM_PATH                 := $(abspath $(CUR_PATH)/../../comm)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../..)

include $(HOME_PATH)/build/color.mk
include $(HOME_PATH)/build/config.mak

MSP_OUT_PATH              := $(HOME_PATH)/msp/out
MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib
MSP_HEADER_EXTERNAL_PATH  := $(HOME_PATH)/header/external
MSP_HEADER_INTERNAL_PATH  := $(HOME_PATH)/header/internal

AXCL_HOME_PATH            := $(HOME_PATH)/axcl
AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal
SPDLOG_LIB_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/lib
SPDLOG_INC_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/include

PROTO_BUF_INC_PATH        := $(AXCL_HOME_PATH)/3rdparty/protobuf/arm64/include
THIRD_PARTY_PATH          := $(HOME_PATH)/third-party

BIN_PATH                  := $(CUR_PATH)/../bin
PRJ_OUT_HOME              := $(HOME_PATH)/build/out/$(PROJECT)
OBJ_OUT_PATH              := $(PRJ_OUT_HOME)/objs
SRC_RELATIVE_PATH         := $(subst $(HOME_PATH)/,,$(CUR_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
ROOTFS_TARGET_PATH        := $(MSP_OUT_PATH)

# output
MOD_NAME                  := slave_daemon
OUTPUT                    := $(TARGET_OUT_PATH)/obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(CUR_PATH)/*.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/threadx.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/event.cpp)

SRCCPPS                   += $(wildcard $(CUR_PATH)/device/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/port/*.cpp)

CINCLUDE                  := -I$(MSP_HEADER_EXTERNAL_PATH) \
                             -I$(COMM_PATH)/include \
                             -I$(AXCL_HOME_PATH)/protocol \
                             -I$(AXCL_HOME_PATH)/protocol/include \
                             -I$(AXCL_HOME_PATH)/comm/include \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/logger \
                             -I$(SPDLOG_INC_PATH) \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(PROTO_BUF_INC_PATH)

CINCLUDE                  += -I$(CUR_PATH)/device
CINCLUDE                  += -I$(CUR_PATH)/port

OBJS                      := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)
OBJS                      += $(SRCS:%.c=$(OUTPUT)/%.o)
DEPS                      := $(OBJS:%.o=%.d)

# exec
TARGET			          := $(TARGET_OUT_PATH)/$(MOD_NAME)

# build flags
CPPFLAGS                  := --std=c++17 -Wall -Wl,-rpath-link=$(MSP_LIB_PATH):$(SPDLOG_LIB_PATH)
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
CLIB                      += -L$(MSP_LIB_PATH) -lax_logger
CLIB                      += -L$(SPDLOG_LIB_PATH) -lspdlog

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_DIR		          := $(ROOTFS_TARGET_PATH)/bin/axclSlave

# link
LINK = $(CC)

#.NOTPARALLEL: clean install
include $(HOME_PATH)/build/rules.mak
