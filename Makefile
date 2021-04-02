TARGET_NAME = whereami

SOURCE_DIR = src
SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.nim)
MAIN_SOURCE = src/whereami.nim

.SILENT: style
.PHONY: style
style:
	nimpretty $(SOURCE_FILES)

.SILENT: run
.PHONY: run
run:
	nimble run $(TARGET_NAME)

.SILENT: build
.PHONY: build
build:
	nimble build $(TARGET_NAME)

.SILENT: release
.PHONY: release
release:
	nimble -y --passL:-s --gc:orc -o:$(TARGET_NAME) compile $(MAIN_SOURCE)
