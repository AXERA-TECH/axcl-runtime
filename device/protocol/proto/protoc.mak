CUR_PATH               := $(shell pwd)
SRC_PATH               := $(abspath $(CUR_PATH)/../../../protocol/proto)
AXCL_HOME_PATH         := $(abspath $(CUR_PATH)/../../..)
HOME_PATH              := $(abspath $(CUR_PATH)/../../../..)
AXCL_PROTOBUF_PATH     := $(AXCL_HOME_PATH)/3rdparty/protobuf
PROTOC                 := $(AXCL_PROTOBUF_PATH)/x64/bin/protoc

include $(HOME_PATH)/build/config.mak
include $(CUR_PATH)/../config.mak

BUILD_OUT_DIR          := $(HOME_PATH)/build/out/$(PROJECT)
PROTO_FILES_DIR        := $(SRC_PATH)
PROTO_FILES            := $(shell find $(PROTO_FILES_DIR) -name "*.proto")
PROTO_INCLUDE_DIR      := $(abspath $(PROTO_FILES_DIR)/../include)
PROTO_OUT_DIR          := $(BUILD_OUT_DIR)/proto


.PHONY: clean all prepare gen install
.NOTPARALLEL:

all: gen

prepare:
	@echo "-- copying *.proto to $(PROTO_OUT_DIR) ..."
	@mkdir -p $(PROTO_OUT_DIR)
	@for proto in $(PROTO_FILES); do \
		rel_file=$$(echo $$proto | sed "s|$(PROTO_FILES_DIR)/||"); \
		dest_path=$(PROTO_OUT_DIR)/$$rel_file; \
		dest_dir=$$(dirname $$dest_path); \
		mkdir -p $$dest_dir; \
		cp -f $$proto $$dest_path; \
	done
	@echo "-- replacing option optimize_for = $(PROTO_OPTIMIZE_OPTION) ..."
	@find $(PROTO_OUT_DIR) -name "*.proto" -exec sed -i 's/option optimize_for = @AXCL_PROTOCOL_OPTIMIZED_OPTION@;/option optimize_for = $(PROTO_OPTIMIZE_OPTION);/g' {} \;

gen: prepare
	@PROTO_FILES=$$(find $(PROTO_OUT_DIR) -name "*.proto"); \
	for proto in $$PROTO_FILES; do \
		rel_file=$$(echo $$proto | sed "s|$(PROTO_OUT_DIR)/||"); \
		echo "-- compile $$proto ..."; \
		$(PROTOC) --cpp_out=$(BUILD_OUT_DIR) -I=$(PROTO_OUT_DIR) $$proto; \
	done

	@mkdir -p $(PROTO_INCLUDE_DIR)
	@PROTO_HEAD_FILES=$$(find $(BUILD_OUT_DIR)/protocol -name "*.h"); \
	for header in $$PROTO_HEAD_FILES; do \
		rel_path=$$(echo $$header | sed "s|$(BUILD_OUT_DIR)/||"); \
		dest_path=$(PROTO_INCLUDE_DIR)/$$rel_path; \
		dest_dir=$$(dirname $$dest_path); \
		mkdir -p $$dest_dir; \
		echo "-- copy $$header to $$dest_path ..."; \
		cp -f $$header $$dest_path; \
	done

install:

clean:
	@rm -rf $(PROTO_OUT_DIR)
	@rm -rf $(BUILD_OUT_DIR)/protocol
	@rm -rf $(PROTO_INCLUDE_DIR)/protocol
