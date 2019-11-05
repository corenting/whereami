TARGET_NAME = whereami

SOURCE_DIR = src
SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.nim)

.SILENT: lint
.PHONY: lint
lint:
	nimpretty $(SOURCE_FILES)

.SILENT: run
.PHONY: run
run:
	nimble run $(TARGET_NAME)

.SILENT: build-release
.PHONY: build-release
build-release:
	nimble -d:release --opt:size --passL:-s build
