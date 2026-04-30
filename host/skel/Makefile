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
MOD_NAME                  := axcl_skel
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(SRC_PATH)/src/*.cpp) \
                             $(wildcard $(SRC_PATH)/src/api/*.cpp) \
                             $(wildcard $(SRC_PATH)/src/mgr/*.cpp) \
                             $(wildcard $(SRC_PATH)/src/impl/*.cpp) \
                             $(wildcard $(SRC_PATH)/src/hvcp/*.cpp) \
                             $(wildcard $(SRC_PATH)/src/face/*.cpp) \
                             $(wildcard $(SRC_PATH)/src/track/*.cpp) \
                             $(wildcard $(SRC_PATH)/src/utils/*.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/dma_buffer.cpp)

CINCLUDE                  := -I$(SRC_PATH)/inc \
                             -I$(AXCL_HEADER_EXTERNAL_PATH) \
                             -I$(AXCL_HEADER_INTERNAL_PATH) \
                             -I$(SRC_PATH)/src/api \
                             -I$(SRC_PATH)/src/mgr \
                             -I$(SRC_PATH)/src/impl \
                             -I$(SRC_PATH)/src/hvcp \
                             -I$(SRC_PATH)/src/face \
                             -I$(SRC_PATH)/src/track \
                             -I$(SRC_PATH)/src/utils \
                             -I$(SRC_PATH)/src/base \
                             -I$(SRC_PATH)/src/cv \
                             -I$(SRC_PATH)/src/middleware \
                             -I$(SRC_PATH)/src/utilities \
                             -I$(SRC_PATH)/src/3rdparty/eigen3 \
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
CFLAGS                    += -fvisibility=hidden
CFLAGS                    += -Werror -Wunused-function
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"
CFLAGS                    += -DAXCL_MODULE_NAME=\"$(SONAME)\"

CFLAGS                    += -Wall -O2 -ggdb3

# dependency
CLIB                      := -lstdc++
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_rt -laxcl_sys -laxcl_ivps -laxcl_logger

# install
INSTALL_LIB               := $(STRIPPED_TARGET) $(DEBUG_TARGET)
MV_TARGET                 := $(INSTALL_LIB)
INSTALL_SKEL_MODELS       := $(SRC_PATH)/src/models/*

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
