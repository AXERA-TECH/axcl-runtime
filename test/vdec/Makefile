CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)

include $(AXCL_HOME_PATH)/build/config.mak

FFMPEG_LIB_PATH           := $(AXCL_LIB_PATH)/ffmpeg
FFMPEG_INC_PATH           := $(AXCL_HOME_PATH)/3rdparty/ffmpeg/$(ARCH)/include

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

# output
MOD_NAME                  := axcl_ut_vdec
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# standard stream script
VDEC_STDSTRM_SCRIPT := vdec_stdStrm_test.sh

# source
SRCS                      := $(wildcard $(SRC_PATH)/*.c) \
                             $(wildcard $(SRC_PATH)/common/*.c)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(SRC_PATH)/common \
                             -I$(AXCL_INC_PATH) \
                             -I$(AXCL_HOME_PATH)/sample \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(FFMPEG_INC_PATH) \
                             -I$(GTEST_INC_PATH) \
                             -I$(AXCL_HOME_PATH)/3rdparty/cmdline/include

OBJS                      := $(SRCS:%.c=$(OUTPUT)/%.o)


DEPS                      := $(OBJS:%.o=%.d)

# exec
TARGET                    := $(TARGET_OUT_PATH)/$(MOD_NAME)

# build flags
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"

ifeq ($(debug),yes)
CFLAGS                    += -Wall -Werror -O0 -ggdb3
else
CFLAGS                    += -Wall -Werror -O2
endif

# dependency
CLIB                      := -lstdc++ -pthread
CLIB                      += -Wl,-rpath-link=$(AXCL_LIB_PATH):$(FFMPEG_LIB_PATH)
CLIB                      += -L$(FFMPEG_LIB_PATH) -lavcodec -lavutil -lavformat
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_rt -laxcl_sys -laxcl_vdec

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_TARGET            += $(VDEC_STDSTRM_SCRIPT)

INSTALL_DIR               := $(MOD_TARGET_PATH)/bin/ut

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
