# Minimal makefile for Sphinx documentation
#
CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)
AXCL_HOME_PATH            := $(abspath $(CUR_PATH)/..)

include $(AXCL_HOME_PATH)/build/version.mak

TAR_NAME   := build/docs-$(SDK_VERSION).tgz
TARGET_DIR := docs-$(SDK_VERSION)

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build

# Put it first so that "make" without argument is like "make help".
help:
	@python3 -m sphinx -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
clean: Makefile
	@python3 -m sphinx -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
	@rm build -rf

html:
	@echo "sphinx build..."
	@python3 -m sphinx -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

install:
	@tar -zcf $(TAR_NAME) --transform="s|^html|$(TARGET_DIR)|" -C build html
