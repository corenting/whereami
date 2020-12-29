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

.SILENT: build-linux
.PHONY: build-linux
build-linux:
	nimble -y -d:release --passL:-s --gc:orc -o:$(TARGET_NAME)_linux compile $(MAIN_SOURCE)

.SILENT: build-win
.PHONY: build-win
build-win:
	nimble -y -d:mingw --passL:-s --gc:orc -o:$(TARGET_NAME)_win compile $(MAIN_SOURCE)

.SILENT: build
.PHONY: build
build: build-linux build-win
