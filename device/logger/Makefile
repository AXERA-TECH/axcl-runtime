COMPILER_USERNAME_STR     := \"$(shell whoami | sed 's/\\/\\\\/g')\"
CUR_PATH                  := $(shell pwd)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../..)

include $(HOME_PATH)/build/color.mk
include $(HOME_PATH)/build/config.mak

MSP_OUT_PATH              := $(HOME_PATH)/msp/out
MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib
MSP_HEADER_EXTERNAL_PATH  := $(HOME_PATH)/header/external
MSP_HEADER_INTERNAL_PATH  := $(HOME_PATH)/header/internal

AXCL_HOME_PATH            := $(HOME_PATH)/axcl
SPDLOG_LIB_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/lib
SPDLOG_INC_PATH           := $(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/include
AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal

PRJ_OUT_HOME              := $(HOME_PATH)/build/out/$(PROJECT)
OBJ_OUT_PATH              := $(PRJ_OUT_HOME)/objs
SRC_RELATIVE_PATH         := $(subst $(HOME_PATH)/,,$(CUR_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
ROOTFS_TARGET_PATH        := $(MSP_OUT_PATH)
GENERATED_PATH            := $(TARGET_OUT_PATH)/generated

soversion = yes

# output
MOD_NAME                  := ax_logger
OUTPUT                    := $(TARGET_OUT_PATH)/obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(CUR_PATH)/version.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/logger/*.cpp)

CINCLUDE                  := -I$(AXCL_HOME_PATH)/logger \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(SPDLOG_INC_PATH) \
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
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"
CFLAGS                    += -DAXCL_MODULE_NAME=\"$(SONAME)\"
CFLAGS                    += -DCOMPILER_USERNAME=$(COMPILER_USERNAME_STR)
CFLAGS                    += -fvisibility=hidden
CFLAGS                    += -Werror -Wunused-function

CFLAGS                    += -Wall -O2 -ggdb3

# dependency
CLIB                      := -lstdc++
CLIB                      += -L$(SPDLOG_LIB_PATH) -lspdlog

# install
INSTALL_LIB               := $(STRIPPED_TARGET) $(DEBUG_TARGET)
MV_TARGET                 := $(INSTALL_LIB)

# link
LINK = $(CC)

#.NOTPARALLEL: clean install
include $(HOME_PATH)/build/rules.mak
