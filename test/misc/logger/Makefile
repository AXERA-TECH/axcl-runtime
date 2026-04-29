CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../../..)

include $(HOME_PATH)/axcl/build/config.mak


GTEST_LIB_PATH            := $(AXCL_HOME_PATH)/3rdparty/googletest/$(ARCH)/lib
GTEST_INC_PATH            := $(AXCL_HOME_PATH)/3rdparty/googletest/$(ARCH)/include

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

# output
MOD_NAME                  := axcl_ut_logger
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(SRC_PATH)/*.cpp)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(GTEST_INC_PATH) \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/logger \
                             -I$(AXCL_HOME_PATH)/3rdparty/spdlog/$(ARCH)/include


OBJS                      := $(SRCS:%.c=$(OUTPUT)/%.o)
CPPOBJS                   := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)

DEPS                      := $(OBJS:%.o=%.d)
CPPDEPS                   := $(CPPOBJS:%.o=%.d)

# exec
TARGET                    := $(TARGET_OUT_PATH)/$(MOD_NAME)

# build flags
CPPFLAGS                  := --std=c++17
CFLAGS                    += $(CPPFLAGS)
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"

CFLAGS                    += -Wall -O2 -ggdb3

# dependency
CLIB                      := -lstdc++ -pthread
CLIB                      += -L$(GTEST_LIB_PATH) -lgtest -lgtest_main -lm
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_logger

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_DIR               := $(MOD_TARGET_PATH)/bin/ut/

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
