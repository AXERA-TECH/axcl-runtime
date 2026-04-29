CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../..)

include $(HOME_PATH)/axcl/build/config.mak
include $(HOME_PATH)/axcl/protocol/config.mak

PROTO_BUF_LIB_PATH        := $(AXCL_HOME_PATH)/3rdparty/protobuf/$(ARCH)/lib
PROTO_BUF_INC_PATH        := $(AXCL_HOME_PATH)/3rdparty/protobuf/$(ARCH)/include
PROTO_CC_OUT_PATH         := $(AXCL_HOME_PATH)/build/out

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

# output
MOD_NAME                  := axcl_proto
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      :=
SRCCPPS				      := $(shell find $(PROTO_CC_OUT_PATH) -name "*.pb.cc")
# $(info SRCCPPS = $(SRCCPPS))

CINCLUDE                  := -I$(PROTO_CC_OUT_PATH) \
                             -I$(PROTO_BUF_INC_PATH)

OBJS                      := $(SRCCPPS:%.cc=$(OUTPUT)/%.o)
OBJS                      += $(SRCS:%.c=$(OUTPUT)/%.o)
DEPS                      := $(OBJS:%.o=%.d)

# static lib
LIBNAME                   := lib$(MOD_NAME).a
STRIPPED_STATIC_TARGET    := $(LIBNAME)

# build flags
CFLAGS                    := $(STATIC_FLAG)
CFLAGS                    += -Wall -O2 -ggdb3

# dependency
CLIB                      := -lstdc++

ifeq ($(PROTO_OPTIMIZE_OPTION), LITE_RUNTIME)
$(info ====>   link libprotobuf-lite.a)
CLIB                      += -L$(PROTO_BUF_LIB_PATH) -l:libprotobuf-lite.a
else
$(info ====>   link libprotobuf.a)
CLIB                      += -L$(PROTO_BUF_LIB_PATH) -l:libprotobuf.a
endif

# install
INSTALL_LIB               := $(STRIPPED_STATIC_TARGET)
MV_TARGET                 := $(INSTALL_LIB)

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
