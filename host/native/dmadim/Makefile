CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)
NATIVE_PATH               := $(abspath $(CUR_PATH)/..)

include $(AXCL_HOME_PATH)/build/config.mak

AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

# output
MOD_NAME                  := axcl_dmadim
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(SRC_PATH)/*.cpp)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(NATIVE_PATH)/template \
                             -I$(AXCL_HOME_PATH)/protocol \
                             -I$(AXCL_HOME_PATH)/protocol/include \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/logger \
                             -I$(AXCL_HOME_PATH)/3rdparty/spdlog/$(ARCH)/include

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
CFLAGS                    += -fvisibility=hidden
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"
CFLAGS                    += -DAXCL_MODULE_NAME=\"$(SONAME)\"

CFLAGS                    += -Wall -O2 -ggdb3

# dependency
CLIB                      := -lstdc++
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_rt -laxcl_pkg -laxcl_token -laxcl_logger

# install
INSTALL_LIB               := $(STRIPPED_TARGET) $(DEBUG_TARGET)
MV_TARGET                 := $(INSTALL_LIB)

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
