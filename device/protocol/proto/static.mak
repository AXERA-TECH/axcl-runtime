CUR_PATH                  := $(shell pwd)
AXCL_HOME_PATH            := $(abspath $(CUR_PATH)/../../..)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../../..)

include $(HOME_PATH)/build/config.mak
include $(CUR_PATH)/../config.mak

PROTO_BUF_LIB_PATH        := $(AXCL_HOME_PATH)/3rdparty/protobuf/arm64/lib
PROTO_BUF_INC_PATH        := $(AXCL_HOME_PATH)/3rdparty/protobuf/arm64/include
PROTO_CC_OUT_PATH         := $(HOME_PATH)/build/out/$(PROJECT)

MSP_OUT_PATH              := $(HOME_PATH)/msp/out
MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib
MSP_INC_PATH              := $(HOME_PATH)/msp/out/include
MSP_HEADER_EXTERNAL_PATH  := $(HOME_PATH)/header/external
MSP_HEADER_INTERNAL_PATH  := $(HOME_PATH)/header/internal

PRJ_OUT_HOME              := $(HOME_PATH)/build/out/$(PROJECT)
OBJ_OUT_PATH              := $(PRJ_OUT_HOME)/objs
SRC_RELATIVE_PATH         := $(subst $(HOME_PATH)/,,$(CUR_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
GENERATED_PATH            := $(TARGET_OUT_PATH)/generated
ROOTFS_TARGET_PATH        := $(MSP_OUT_PATH)

# output
MOD_NAME                  := ax_proto
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

include $(HOME_PATH)/build/rules.mak
