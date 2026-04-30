CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)

include $(AXCL_HOME_PATH)/build/config.mak

AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

# output
MOD_NAME                  := axcl_spl_update
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      := $(wildcard $(AXCL_HOME_PATH)/3rdparty/md5/md5c.c)
SRCCPPS                   := $(wildcard $(SRC_PATH)/*.cpp)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(AXCL_HOME_PATH)/3rdparty/md5 \
                             -I$(AXCL_HOME_PATH)/3rdparty/cmdline/include

OBJS                      := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)
OBJS                      += $(SRCS:%.c=$(OUTPUT)/%.o)
DEPS                      := $(OBJS:%.o=%.d)


TARGET                    := $(TARGET_OUT_PATH)/$(MOD_NAME)

# build flags
CPPFLAGS                  := --std=c++17
CFLAGS                    += -DAXCL_SDK_VERSION=\"$(SDK_VERSION)\"

ifeq ($(debug),yes)
CFLAGS                    += -Wall -O0 -ggdb3
else
CFLAGS                    += -Wall -O2
endif

# dependency
CLIB                      := -lstdc++ -pthread -lm
CLIB                      += -Wl,-rpath-link=$(AXCL_LIB_PATH)
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_rt

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_DIR               := $(MOD_TARGET_PATH)/bin/

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
