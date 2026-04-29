CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../../../..)

include $(HOME_PATH)/axcl/build/config.mak

AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal
GTEST_LIB_PATH            := $(AXCL_HOME_PATH)/3rdparty/googletest/$(ARCH)/lib
GTEST_INC_PATH            := $(AXCL_HOME_PATH)/3rdparty/googletest/$(ARCH)/include

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib

# output
MOD_NAME                  := axcl_ut_pcie_rc
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(SRC_PATH)/*.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/elapser.cpp)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(GTEST_INC_PATH) \
                             -I$(AXCL_INC_PATH) \
                             -I$(AXCL_HOME_PATH)/comm/include \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(AXCL_HOME_PATH)/3rdparty/cmdline/include

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

ifeq ($(debug),yes)
CFLAGS                    += -Wall -O0 -ggdb3
else
CFLAGS                    += -Wall -O2
endif

# dependency
CLIB                      := -lstdc++ -pthread
CLIB                      += -L$(GTEST_LIB_PATH) -lgtest -lgtest_main -lm
ifeq ($(HOST),ax650)
CLIB                      += -Wl,-rpath-link=$(AXCL_LIB_PATH):$(MSP_LIB_PATH)
CLIB                      += -L$(MSP_LIB_PATH) -lax_sys
else
CLIB                      += -Wl,-rpath-link=$(AXCL_LIB_PATH)
endif
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_comm

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_DIR               := $(MOD_TARGET_PATH)/bin/ut/

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
