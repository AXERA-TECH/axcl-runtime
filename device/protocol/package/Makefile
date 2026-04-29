COMPILER_USERNAME_STR     := \"$(shell whoami | sed 's/\\/\\\\/g')\"
CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(abspath $(CUR_PATH)/../../../protocol/package)
AXCL_HOME_PATH            := $(abspath $(CUR_PATH)/../../..)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../../..)

include $(HOME_PATH)/build/config.mak
include $(HOME_PATH)/build/color.mk
include $(CUR_PATH)/../config.mak

AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal

PROTO_BUF_LIB_PATH        := $(AXCL_HOME_PATH)/3rdparty/protobuf/arm64/lib
PROTO_BUF_INC_PATH        := $(AXCL_HOME_PATH)/3rdparty/protobuf/arm64/include

SPDLOG_LIB_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/lib

MSP_OUT_PATH              := $(HOME_PATH)/msp/out
MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib
MSP_HEADER_EXTERNAL_PATH  := $(HOME_PATH)/header/external
MSP_HEADER_INTERNAL_PATH  := $(HOME_PATH)/header/internal

PRJ_OUT_HOME              := $(HOME_PATH)/build/out/$(PROJECT)
OBJ_OUT_PATH              := $(PRJ_OUT_HOME)/objs
SRC_RELATIVE_PATH         := $(subst $(HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
ROOTFS_TARGET_PATH        := $(MSP_OUT_PATH)
GENERATED_PATH            := $(TARGET_OUT_PATH)/generated

soversion = yes

# output
MOD_NAME                  := ax_pkg
OUTPUT                    := $(TARGET_OUT_PATH)/obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(CUR_PATH)/version.cpp) \
                             $(wildcard $(SRC_PATH)/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/native/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/native/module/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/runtime/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/runtime/p2p/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/runtime/usrwork/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/runtime/memory/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/runtime/engine/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/runtime/system/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/runtime/system/log/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/runtime/system/latency/*.cpp) \
                             $(wildcard $(SRC_PATH)/host/runtime/system/control/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/native/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/native/module/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/runtime/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/runtime/p2p/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/runtime/usrwork/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/runtime/memory/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/runtime/engine/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/runtime/system/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/runtime/system/log/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/runtime/system/latency/*.cpp) \
                             $(wildcard $(SRC_PATH)/device/runtime/system/control/*.cpp)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(AXCL_HOME_PATH)/protocol \
                             -I$(AXCL_HOME_PATH)/protocol/include \
                             -I$(PROTO_BUF_INC_PATH) \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/logger \
                             -I$(MSP_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
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
CPPFLAGS                  := --std=c++17
CFLAGS                    := $(DYNAMIC_FLAG) $(CPPFLAGS)
CFLAGS                    += -Werror -Wunused-function
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"
CFLAGS                    += -DAXCL_MODULE_NAME=\"$(SONAME)\"
CFLAGS                    += -DCOMPILER_USERNAME=$(COMPILER_USERNAME_STR)

CFLAGS                    += -Wall -O2 -ggdb3

# dependency
CLIB                      := -lstdc++
CLIB                      += -L$(MSP_LIB_PATH) -l:libax_proto.a
CLIB                      += -L$(MSP_LIB_PATH) -lax_logger

ifeq ($(PROTO_OPTIMIZE_OPTION), LITE_RUNTIME)
$(info ====>   link libprotobuf-lite.a)
CLIB                      += -L$(PROTO_BUF_LIB_PATH) -l:libprotobuf-lite.a
else
$(info ====>   link libprotobuf.a)
CLIB                      += -L$(PROTO_BUF_LIB_PATH) -l:libprotobuf.a
endif
CLIB                      += -Wl,--exclude-libs,ALL


# install
INSTALL_LIB               := $(STRIPPED_TARGET) $(DEBUG_TARGET)
MV_TARGET                 := $(INSTALL_LIB)

# link
LINK = $(CC)

include $(HOME_PATH)/build/rules.mak
