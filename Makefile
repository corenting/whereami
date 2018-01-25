TARGET = whereami
FOLDER = /usr/local/bin

CC = gcc
CFLAGS = -pedantic -Wextra -Wall -Wextra -Werror -O2
LDFLAGS = -lcurl -lm

SRC = $(shell find src -type f -iname '*.c')
OBJS = $(foreach x, $(basename $(SRC)), $(x).o)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

.PHONY: clean
clean:
	rm -f ${OBJS} ${TARGET}

.PHONY: install
install:
	install $(TARGET) $(FOLDER)
