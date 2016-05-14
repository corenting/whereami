TARGET = whereami
FOLDER = /usr/local/bin

CC = gcc
CFLAGS = -Wall -Wextra -O3
LDFLAGS = 
LIBS = -lcurl
 
SRC = $(shell find src -type f -iname '*.c')
OBJS = $(foreach x, $(basename $(SRC)), $(x).o)
 
$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)
 
.PHONY: clean
clean:
	rm -f ${OBJ} ${TARGET}
	
.PHONY: install
install:
	install $(TARGET) $(FOLDER)