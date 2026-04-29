MKFILE_PATH               := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_PATH                  := $(abspath $(dir $(MKFILE_PATH)))
SRC_PATH                  := $(CUR_PATH)
AXCL_HOME_PATH            := $(abspath $(CUR_PATH)/../../..)
HOME_PATH                 := $(abspath $(AXCL_HOME_PATH)/..)

include $(AXCL_HOME_PATH)/build/config.mak

AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal
HOME_HEADER_EXTERNAL_PATH := $(HOME_PATH)/header/external

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

# output
MOD_NAME			      := axcl_pcie_dma

OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      := $(wildcard $(SRC_PATH)/*.c)
SRCCPPS                   := $(wildcard $(AXCL_HOME_PATH)/drv/pcie/log/*.cpp)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/logger \
                             -I$(AXCL_HOME_PATH)/3rdparty/spdlog/$(ARCH)/include \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(HOME_HEADER_EXTERNAL_PATH)

OBJS                      := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)
OBJS                      += $(SRCS:%.c=$(OUTPUT)/%.o)
DEPS                      := $(OBJS:%.o=%.d)

# dynamic lib
SONAME                    := lib$(MOD_NAME).so
STRIPPED_TARGET           := $(SONAME)
DEBUG_TARGET              := lib$(MOD_NAME).debug

# build flags
CPPFLAGS                  := --std=c++17

CFLAGS                    := $(DYNAMIC_FLAG)
CFLAGS                    += -Werror
CFLAGS                    += -fvisibility=hidden
CFLAGS                    += -DAXCL_HOST
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"
CFLAGS                    += -DAXCL_MODULE_NAME=\"$(SONAME)\"

CFLAGS                    += -Wall -O2 -ggdb3

# dependency
CLIB                      := -lstdc++
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_logger

# install
INSTALL_LIB               := $(STRIPPED_TARGET) $(DEBUG_TARGET)
MV_TARGET                 := $(INSTALL_LIB)

# link
LINK                       = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak