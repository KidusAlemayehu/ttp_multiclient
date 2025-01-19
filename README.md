### Tiny TLS Proxy

A multi-client transparent TLS proxy with rate limiting, designed to demonstrate secure communication using OpenSSL. The project securely forwards traffic between clients and backend servers with TLS encryption.

## Features

- **TLS encryption** for secure communication using OpenSSL
- **Rate limiting** to control client bandwidth usage
- **Multi-client handling with threads**
- **Dynamic message exchange**: Reads and writes data over a network connection
- **Real-time message processing between client and server**
- **Go-based client for concurrent request testing**

## Inspirations

- **[Tiny TLS Proxy](https://github.com/Theldus/ttp)**: This project was inspired by the [Tiny TLS Proxy](https://github.com/Theldus/ttp), emphasizing a straightforward but secure implementation.
- **Reddit Post**: Design and implementation ideas were guided by a [Reddit discussion on C network programming](https://www.reddit.com/r/networking/comments/xxxxxx).

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/KidusAlemayehu/ttp_multiclient.git
   cd ttp_multiclient
   ```


2. Compile the project:
   ```bash
   make
   ```

## Usage

### Running the Server

Start the server:

```bash
./ttp_server
```

This will launch the server, listening on port `8050` for incoming connections.

### Running the Client

Run the client:

```bash
./ssl_client
```

This client connects to the server and dynamically sends and receives data over the network. It simulates concurrent client connections and rate-limiting behavior.

### Go Client (for Concurrent Requests)

You can also test concurrent requests using the Go client:

1. Run the Go client:
   ```bash
   ./ssl_client_go
   ```

This client will send concurrent requests with different messages to the server to test its handling of multiple client connections.

## Project Structure

```
ttp_multiclient/
├── src/               # Server source code
├── client/            # C Client source code
├── go-client/         # Go Client for concurrent requests
├── include/           # Header files
├── certs/             # TLS certificates (excluded in .gitignore)
├── README.md          # Project documentation
├── Makefile           # Build instructions
└── .gitignore         # Ignored files
```

## SSL Certificates

To use TLS encryption, generate a certificate (`server.crt`) and a private key (`server.key`):

```bash
openssl genpkey -algorithm RSA -out server.key
openssl req -new -key server.key -out server.csr
openssl x509 -req -in server.csr -signkey server.key -out server.crt
```

Place these files in the `certs/` directory.

## Makefile

The project is built using `make`. The Makefile supports building the server and client applications.

```makefile
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

```

## License

MIT License

Copyright (c) 2025 Kidus Alemayehu

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
