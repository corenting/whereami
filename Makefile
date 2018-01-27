TARGET = whereami
FOLDER = /usr/local/bin

CC = gcc
CFLAGS = -std=gnu11 -pedantic -Werror -Wextra -Wall -Wextra -O2
LIBS = -lcurl -lm

SRC = $(shell find src -type f -iname '*.c')
OBJS = $(foreach x, $(basename $(SRC)), $(x).o)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(TARGET) $(OBJS)

.PHONY: clean
clean:
	rm -f ${OBJS} ${TARGET}

.PHONY: install
install:
	install $(TARGET) $(FOLDER)
