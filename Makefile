CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lssl -lcrypto

SRC = src/main.c src/socket_utils.c
OBJ = $(SRC:.c=.o)
SERVER_TARGET = ttp_server

CLIENT_SRC = client/client.c
CLIENT_TARGET = ssl_client

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(CLIENT_TARGET): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(SERVER_TARGET) $(CLIENT_TARGET)
