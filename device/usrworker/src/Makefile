CUR_PATH                  := $(shell pwd)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../../..)

include $(HOME_PATH)/build/color.mk
include $(HOME_PATH)/build/config.mak

MSP_OUT_PATH              := $(HOME_PATH)/msp/out
MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib
MSP_INC_PATH              := $(HOME_PATH)/msp/out/include
MSP_HEADER_EXTERNAL_PATH  := $(HOME_PATH)/header/external
MSP_HEADER_INTERNAL_PATH  := $(HOME_PATH)/header/internal

AXCL_HOME_PATH            := $(HOME_PATH)/axcl
AXCL_BUILD_PATH           := $(AXCL_HOME_PATH)/build
AXCL_LIB_PATH             := $(AXCL_BUILD_PATH)/lib
AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal
SPDLOG_LIB_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/lib

PRJ_OUT_HOME              := $(HOME_PATH)/build/out/$(PROJECT)
OBJ_OUT_PATH              := $(PRJ_OUT_HOME)/objs
SRC_RELATIVE_PATH         := $(subst $(HOME_PATH)/,,$(CUR_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
ROOTFS_TARGET_PATH        := $(MSP_OUT_PATH)
GENERATED_PATH            := $(TARGET_OUT_PATH)/generated

soversion = yes

# output
MOD_NAME                  := axclrt_worker
OUTPUT                    := $(TARGET_OUT_PATH)/obj

# source
SRCS                      :=
SRCCPPS                   += $(wildcard $(CUR_PATH)/version.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/usrworker_ipc.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/usrworker.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/usrworker_runtime.cpp)
SRCCPPS                   += $(wildcard $(CUR_PATH)/usrworker_channel.cpp)
SRCCPPS                   += $(wildcard $(AXCL_HOME_PATH)/toolkit/threadx.cpp)
SRCCPPS                   += $(wildcard $(AXCL_HOME_PATH)/toolkit/event.cpp)

CINCLUDE                  := -I$(CUR_PATH) \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/logger \
                             -I$(AXCL_HOME_PATH)/comm/include \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/include \
                             -I$(GENERATED_PATH)

OBJS                      := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)
OBJS                      += $(SRCS:%.c=$(OUTPUT)/%.o)
DEPS                      := $(OBJS:%.o=%.d)

# dynamic lib version (must)
SONAME                    := lib$(MOD_NAME).so
STRIPPED_TARGET           := $(SONAME)
DEBUG_TARGET              := lib$(MOD_NAME).debug

# build flags
CPPFLAGS                  := --std=c++17 -Wall -Wl,-rpath-link=$(MSP_LIB_PATH)
CFLAGS                    := $(DYNAMIC_FLAG) $(CPPFLAGS)
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"
CFLAGS                    += -DAXCL_MODULE_NAME=\"$(SONAME)\"
CFLAGS                    += -fvisibility=hidden
CFLAGS                    += -Werror -Wunused-function

ifeq ($(debug),yes)
CFLAGS                    += -Wall -O0 -ggdb3
else
CFLAGS                    += -Wall -O2
endif

# dependency
CLIB                      := -lstdc++ -pthread -lrt
CLIB                      += -L$(MSP_LIB_PATH) -lax_comm -lax_logger

# install
INSTALL_LIB               := $(STRIPPED_TARGET) $(DEBUG_TARGET)
MV_TARGET                 := $(INSTALL_LIB)

# link
LINK = $(CC)

#.NOTPARALLEL: clean install
include $(HOME_PATH)/build/rules.mak