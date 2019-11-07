TARGET_NAME = whereami

SOURCE_DIR = src
SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.nim)
MAIN_SOURCE = src/whereami.nim

.SILENT: lint
.PHONY: lint
lint:
	nimpretty $(SOURCE_FILES)

.SILENT: run
.PHONY: run
run:
	nimble run $(TARGET_NAME)

.SILENT: build-release-linux
.PHONY: build-release-linux
build-release-linux:
	nimble -y -d:release --opt:size --passL:-s -o:$(TARGET_NAME)_linux compile $(MAIN_SOURCE)

.SILENT: build-release-win
.PHONY: build-release-win
build-release-win:
	nimble -y -d:mingw -d:release --opt:size --passL:-s -o:$(TARGET_NAME)_win compile $(MAIN_SOURCE)

.SILENT: build-release-all
.PHONY: build-release-all
build-release-all: build-release-linux build-release-win
