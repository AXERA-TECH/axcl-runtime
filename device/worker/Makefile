CUR_PATH                  := $(shell pwd)
COMM_PATH                 := $(abspath $(CUR_PATH)/../../comm)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../..)

include $(HOME_PATH)/build/color.mk
include $(HOME_PATH)/build/config.mak

MSP_OUT_PATH              := $(HOME_PATH)/msp/out
MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib
MSP_INC_PATH              := $(HOME_PATH)/msp/out/include
MSP_HEADER_EXTERNAL_PATH  := $(HOME_PATH)/header/external
MSP_HEADER_INTERNAL_PATH  := $(HOME_PATH)/header/internal

AXCL_HOME_PATH            := $(HOME_PATH)/axcl
AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal
SPDLOG_INC_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/include
SPDLOG_LIB_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/lib
PROTO_BUF_INC_PATH        := $(AXCL_HOME_PATH)/3rdparty/protobuf/arm64/include
THIRD_PARTY_PATH          := $(HOME_PATH)/third-party

BIN_PATH                  := $(CUR_PATH)/../bin
PRJ_OUT_HOME              := $(HOME_PATH)/build/out/$(PROJECT)
OBJ_OUT_PATH              := $(PRJ_OUT_HOME)/objs
SRC_RELATIVE_PATH         := $(subst $(HOME_PATH)/,,$(CUR_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
ROOTFS_TARGET_PATH        := $(MSP_OUT_PATH)


# output
MOD_NAME                  := slave_worker
OUTPUT                    := $(TARGET_OUT_PATH)/obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(CUR_PATH)/*.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/threadx.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/event.cpp)

SRCCPPS                   += $(wildcard $(CUR_PATH)/device/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/port/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/channel/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/dispatch/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/context/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/stream/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/event/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/task/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/impl/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/runtime/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/runtime/memory/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/runtime/engine/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/runtime/system/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/runtime/p2p/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/runtime/usrwork/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/native/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/native/dmadim/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/native/npu/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/native/ive/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/native/ivps/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/native/sys/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/native/vdec/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/native/venc/*.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/native/dsp/*.cpp)
SRCCPPS                   += $(wildcard $(AXCL_HOME_PATH)/device/usrworker/src/usrworker_ipc.cpp)
SRCCPPS                   += $(wildcard $(AXCL_HOME_PATH)/device/usrworker/src/usrworker_manager.cpp)


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
                             -I$(PROTO_BUF_INC_PATH) \
                             -I$(AXCL_HOME_PATH)/device/usrworker/src

CINCLUDE                  += -I$(CUR_PATH)/device
CINCLUDE                  += -I$(CUR_PATH)/port
CINCLUDE                  += -I$(CUR_PATH)/channel
CINCLUDE                  += -I$(CUR_PATH)/dispatch
CINCLUDE                  += -I$(CUR_PATH)/context
CINCLUDE                  += -I$(CUR_PATH)/stream
CINCLUDE                  += -I$(CUR_PATH)/event
CINCLUDE                  += -I$(CUR_PATH)/task
CINCLUDE                  += -I$(CUR_PATH)/impl
CINCLUDE                  += -I$(CUR_PATH)/base
CINCLUDE                  += -I$(CUR_PATH)/runtime
CINCLUDE                  += -I$(CUR_PATH)/runtime/memory
CINCLUDE                  += -I$(CUR_PATH)/runtime/engine
CINCLUDE                  += -I$(CUR_PATH)/runtime/system
CINCLUDE                  += -I$(CUR_PATH)/runtime/p2p
CINCLUDE                  += -I$(CUR_PATH)/runtime/usrwork
CINCLUDE                  += -I$(CUR_PATH)/native
CINCLUDE                  += -I$(CUR_PATH)/native/dmadim
CINCLUDE                  += -I$(CUR_PATH)/native/npu
CINCLUDE                  += -I$(CUR_PATH)/native/ive
CINCLUDE                  += -I$(CUR_PATH)/native/ivps
CINCLUDE                  += -I$(CUR_PATH)/native/sys
CINCLUDE                  += -I$(CUR_PATH)/native/vdec
CINCLUDE                  += -I$(CUR_PATH)/native/venc
CINCLUDE                  += -I$(CUR_PATH)/native/dsp

OBJS                      := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)
OBJS                      += $(SRCS:%.c=$(OUTPUT)/%.o)
DEPS                      := $(OBJS:%.o=%.d)

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
CLIB                      := -lstdc++ -pthread -lrt
CLIB                      += -L$(MSP_LIB_PATH)
CLIB                      += -lax_pkg
CLIB                      += -lax_comm
CLIB                      += -lax_logger
CLIB                      += -lax_sys
CLIB                      += -lax_vdec
CLIB                      += -lax_ivps
CLIB                      += -lax_venc
CLIB                      += -lax_engine
CLIB                      += -lax_dmadim
CLIB                      += -lax_ive
CLIB                      += -lax_dsp

CLIB                      += -L$(THIRD_PARTY_PATH)/libexif/lib -lexif
CLIB                      += -L$(SPDLOG_LIB_PATH) -lspdlog

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_TARGET            += $(BIN_PATH)/*
INSTALL_DIR               := $(ROOTFS_TARGET_PATH)/bin/axclSlave

# link
LINK = $(CC)

#.NOTPARALLEL: clean install
include $(HOME_PATH)/build/rules.mak
