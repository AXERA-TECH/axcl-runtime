SUBDIRS = token runtime native smi spl_update

################################################################################
#	prepare param
################################################################################
CUR_PATH                  := $(shell pwd)

include $(AXCL_HOME_PATH)/build/config.mak

################################################################################
#	set task
################################################################################
SUBDIRS_CLEAN   = $(addsuffix .clean, $(SUBDIRS))
SUBDIRS_INSTALL = $(addsuffix .install, $(SUBDIRS))

.PHONY: $(SUBDIRS) $(SUBDIRS_INSTALL) $(SUBDIRS_CLEAN) prepare
.NOTPARALLEL:

all: prepare $(SUBDIRS)
	@$(ECHO) -e $(GREEN)"\nBuild All $(CURDIR) Modules success!!\n"  $(DONE)

install: $(SUBDIRS_INSTALL)
	@$(ECHO) -e $(GREEN)"\nInstall $(CURDIR) success!!\n"  $(DONE)

prepare:
	@$(MKDIR) $(AXCL_INC_PATH)
	@$(CP) $(AXCL_HOME_PATH)/include/external/*.h  $(AXCL_INC_PATH)/

# remove api define in msp api header files
	@sed -i '/^#ifdef __cplusplus$$/,/^#endif \/\/_AX_SYS_API_$$/{ /^#endif \/\/_AX_SYS_API_$$/!d }' $(AXCL_INC_PATH)/ax_sys_api.h
	@sed -i '/^\s*AX_S32 AX_IVPS_Init.*$$/,/^\s*AX_S32 AX_IVPS_GetEngineDutyCycle.*$$/d' $(AXCL_INC_PATH)/ax_ivps_api.h
	@sed -i '/^\s*IVPS_RGN_HANDLE AX_IVPS_RGN_Create.*$$/,/^#ifdef __cplusplus$$/{ /^#ifdef __cplusplus$$/!d }' $(AXCL_INC_PATH)/ax_ivps_api.h
	@sed -i '/^\s*AX_S32 AX_DMADIM_Open.*$$/,/^\s*AX_S32 AX_DMA_CheckSum.*$$/d' $(AXCL_INC_PATH)/ax_dmadim_api.h

clean:	$(SUBDIRS_CLEAN)
	@$(ECHO) -e $(GREEN)"\nClean $(CURDIR) success!!\n"  $(DONE)

$(SUBDIRS):
	@$(ECHO)
	@$(ECHO) -e $(CYAN)"In subdir $@ ..." $(DONE)
	@$(MAKE) -C $(basename $@ )

$(SUBDIRS_INSTALL):
	@$(ECHO)
	@$(ECHO) -e $(CYAN)"In subdir $(basename $@ )..." $(DONE)
	@$(MAKE) -C $(basename $@ ) install

$(SUBDIRS_CLEAN):
	@$(ECHO) -e $(CYAN)"In subdir $(basename $@ )..." $(DONE)
	@$(MAKE) -C $(basename $@ ) clean


# Export val
export MOD_TARGET_PATH
