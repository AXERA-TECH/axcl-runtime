CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)
HOME_PATH                 := $(abspath $(CUR_PATH)/../..)

include $(SRC_PATH)/config.mak
include $(HOME_PATH)/axcl/build/config.mak

AXCL_HEADER_EXTERNAL_PATH := $(AXCL_HOME_PATH)/include/external
AXCL_HEADER_INTERNAL_PATH := $(AXCL_HOME_PATH)/include/internal
HOME_HEADER_EXTERNAL_PATH := $(HOME_PATH)/header/external
ZMQ_LIB_PATH              := $(AXCL_HOME_PATH)/3rdparty/zmq/$(ARCH)

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

# output
MOD_NAME                  := axcl_comm
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(SRC_PATH)/impl/*.cpp) \
                             $(wildcard $(SRC_PATH)/impl/pcie/*.cpp) \
                             $(wildcard $(SRC_PATH)/impl/pcie/handler/*.cpp) \
                             $(wildcard $(SRC_PATH)/impl/socket/*.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/dma_buffer.cpp)

CINCLUDE                  := -I$(SRC_PATH)/impl \
                             -I$(SRC_PATH)/impl/pcie \
                             -I$(SRC_PATH)/impl/pcie/handler \
                             -I$(SRC_PATH)/impl/socket \
                             -I$(SRC_PATH)/include \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/logger \
                             -I$(AXCL_HOME_PATH)/3rdparty/spdlog/$(ARCH)/include \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(HOME_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HOME_PATH)/3rdparty/zmq/include

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
CFLAGS                    += -fvisibility=hidden
CFLAGS                    += -Werror -Wunused-function

ifdef PCIE_DMA_BUFFER_SIZE
CFLAGS                    += -DPCIE_DMA_BUFFER_SIZE=$(PCIE_DMA_BUFFER_SIZE)
else
CFLAGS                    += -DPCIE_DMA_BUFFER_SIZE=0x400000
endif
$(info PCIE_DMA_BUFFER_SIZE = $(PCIE_DMA_BUFFER_SIZE))

CFLAGS                    += -Wall -O2 -ggdb3

# dependency
CLIB                      := -lstdc++ -pthread
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_pcie_msg -laxcl_pcie_dma -laxcl_logger
CLIB                      += -L$(ZMQ_LIB_PATH) -l:libzmq.a
CLIB                      += -Wl,--exclude-libs,ALL

# install
INSTALL_LIB               := $(STRIPPED_TARGET) $(DEBUG_TARGET)
MV_TARGET                 := $(INSTALL_LIB)

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
