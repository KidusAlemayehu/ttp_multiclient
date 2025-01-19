# Compiler and flags for C programs
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lssl -lcrypto

# Server source files
SRC = src/main.c src/socket_utils.c src/logger.c src/connection.c
OBJ = $(SRC:.c=.o)
SERVER_TARGET = ttp_server

# C client
CLIENT_SRC = client/client.c
CLIENT_TARGET = ssl_client

# Go client
GO_CLIENT_SRC = go-client/client.go
GO_CLIENT_TARGET = ssl_client_go

all: $(SERVER_TARGET) $(CLIENT_TARGET) $(GO_CLIENT_TARGET)

# Build server
$(SERVER_TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build C client
$(CLIENT_TARGET): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build Go client
$(GO_CLIENT_TARGET): $(GO_CLIENT_SRC)
	GOOS=$(shell uname -s | tr A-Z a-z) GOARCH=amd64 go build -o $@ $^

# Clean up
clean:
	rm -f $(OBJ) $(SERVER_TARGET) $(CLIENT_TARGET) $(GO_CLIENT_TARGET)
