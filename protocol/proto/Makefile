CUR_PATH            := $(shell pwd)

include $(AXCL_HOME_PATH)/build/config.mak

################################################################################
# build
################################################################################

.PHONY : all clean install lib
.NOTPARALLEL:

all: lib
	@echo -e $(GREEN)"\nBuild $@ success!!\n"  $(DONE)

clean:
	$(VERB) $(MAKE) -f protoc.mak clean
	$(VERB) $(MAKE) -f static.mak clean
	@echo -e $(GREEN)"\nClean $@ success!!\n"  $(DONE)

install:
	@echo -e $(GREEN)"Install $@ ..."  $(DONE)
	$(VERB) $(MAKE) -f protoc.mak install
	$(VERB) $(MAKE) -f static.mak install

lib:
	@echo -e $(GREEN)"Build dynamic $@ ..."  $(DONE)
	$(VERB) $(MAKE) -f protoc.mak
	$(VERB) $(MAKE) -f static.mak

export MOD_TARGET_PATH
