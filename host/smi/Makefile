CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../..)

include $(HOME_PATH)/axcl/build/config.mak

AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal
ARGSXX_PATH               := $(AXCL_HOME_PATH)/3rdparty/argsxx

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib

# output
MOD_NAME                  := axcl-smi
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCCPPS                   := $(wildcard $(SRC_PATH)/*.cpp) \
                             $(wildcard $(SRC_PATH)/*/*.cpp)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(SRC_PATH)/cmd \
                             -I$(SRC_PATH)/utils \
                             -I$(AXCL_INC_PATH) \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(ARGSXX_PATH)


OBJS                      := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)
OBJS                      += $(SRCS:%.c=$(OUTPUT)/%.o)
DEPS                      := $(OBJS:%.o=%.d)


TARGET                    := $(TARGET_OUT_PATH)/$(MOD_NAME)

# build flags
CPPFLAGS                  := --std=c++17
CFLAGS                    += -DAXCL_SMI_VERSION=\"$(SDK_VERSION)\"
CFLAGS                    += -DAXCL_SDK_VERSION=\"$(SDK_VERSION)\"

ifeq ($(debug),yes)
CFLAGS                    += -Wall -O0 -ggdb3
else
CFLAGS                    += -Wall -O2
endif

# dependency
CLIB                      := -lstdc++ -pthread -lm
ifeq ($(HOST),ax650)
CLIB                      += -Wl,-rpath-link=$(AXCL_LIB_PATH):$(MSP_LIB_PATH)
CLIB                      += -L$(MSP_LIB_PATH) -lax_sys
else
CLIB                      += -Wl,-rpath-link=$(AXCL_LIB_PATH)
endif
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_rt -laxcl_npu

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_DIR               := $(MOD_TARGET_PATH)/bin/

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
