CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/socket_utils.c
OBJ = $(SRC:.c=.o)
TARGET= ttp_server

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)