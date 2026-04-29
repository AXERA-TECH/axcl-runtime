COMPILER_USERNAME_STR     := \"$(shell whoami | sed 's/\\/\\\\/g')\"
CUR_PATH                  := $(shell pwd)
COMM_PATH                 := $(abspath $(CUR_PATH)/../../comm)
HOME_PATH                 := $(abspath $(CUR_PATH)/../../..)

include $(COMM_PATH)/config.mak
include $(HOME_PATH)/build/color.mk
include $(HOME_PATH)/build/config.mak

MSP_OUT_PATH              := $(HOME_PATH)/msp/out
MSP_LIB_PATH              := $(HOME_PATH)/msp/out/lib
MSP_HEADER_EXTERNAL_PATH  := $(HOME_PATH)/header/external
MSP_HEADER_INTERNAL_PATH  := $(HOME_PATH)/header/internal

AXCL_HOME_PATH            := $(HOME_PATH)/axcl
AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal
ZMQ_LIB_PATH              := $(AXCL_HOME_PATH)/3rdparty/zmq/arm64

PRJ_OUT_HOME              := $(HOME_PATH)/build/out/$(PROJECT)
OBJ_OUT_PATH              := $(PRJ_OUT_HOME)/objs
SRC_RELATIVE_PATH         := $(subst $(HOME_PATH)/,,$(CUR_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
ROOTFS_TARGET_PATH        := $(MSP_OUT_PATH)
GENERATED_PATH            := $(TARGET_OUT_PATH)/generated

soversion = yes

# output
MOD_NAME                  := ax_comm
OUTPUT                    := $(TARGET_OUT_PATH)/obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(CUR_PATH)/version.cpp) \
                             $(wildcard $(COMM_PATH)/impl/*.cpp) \
                             $(wildcard $(COMM_PATH)/impl/pcie/*.cpp) \
                             $(wildcard $(COMM_PATH)/impl/pcie/handler/*.cpp) \
                             $(wildcard $(COMM_PATH)/impl/socket/*.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/dma_buffer.cpp)

CINCLUDE                  := -I$(COMM_PATH)/impl \
                             -I$(COMM_PATH)/impl/pcie \
                             -I$(COMM_PATH)/impl/pcie/handler \
                             -I$(COMM_PATH)/impl/socket \
                             -I$(COMM_PATH)/include \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/logger \
                             -I$(AXCL_HOME_PATH)/3rdparty/spdlog/arm64/include \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(MSP_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HOME_PATH)/3rdparty/zmq/include \
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
CFLAGS                    += -DAXCL_CMA_CACHED

ifdef PCIE_DMA_BUFFER_SIZE
CFLAGS                    += -DPCIE_DMA_BUFFER_SIZE=$(PCIE_DMA_BUFFER_SIZE)
else
CFLAGS                    += -DPCIE_DMA_BUFFER_SIZE=0x400000
endif
$(info PCIE_DMA_BUFFER_SIZE = $(PCIE_DMA_BUFFER_SIZE))

CFLAGS                    += -Wall -O2 -ggdb3

# dependency
CLIB                      := -lstdc++  -pthread
CLIB                      += -L$(MSP_LIB_PATH) -lax_pcie_msg -lax_pcie_dma -lax_logger
CLIB                      += -L$(ZMQ_LIB_PATH) -l:libzmq.a
CLIB                      += -Wl,--exclude-libs,ALL

# install
INSTALL_LIB               := $(STRIPPED_TARGET) $(DEBUG_TARGET)
MV_TARGET                 := $(INSTALL_LIB)

# link
LINK = $(CC)

#.NOTPARALLEL: clean install
include $(HOME_PATH)/build/rules.mak
