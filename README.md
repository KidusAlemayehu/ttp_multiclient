
# Tiny TLS Proxy

A multi-client transparent TLS proxy with rate limiting, designed to demonstrate secure communication using OpenSSL. The project securely forwards traffic between clients and backend servers with TLS encryption.

## Features

- **TLS encryption** for secure communication using OpenSSL
- **Rate limiting** to control client bandwidth usage
- **Multi-client handling** with threads
- **Dynamic message exchange**: Reads and writes data over a network connection
- **Real-time message processing** between client and server

## Inspirations

- [Tiny TLS Proxy](https://github.com/Theldus/ttp): This project was inspired by the Tiny TLS Proxy, emphasizing a straightforward but secure implementation.
- Reddit Post: Design and implementation ideas were guided by a [Reddit discussion on C network programming](https://www.reddit.com/r/C_Programming/s/XDJswdvydS).

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

The client connects to the server and dynamically sends and receives data over the network.

## Project Structure

```
ttp_multiclient/
├── src/               # Server source code
├── client/            # Client source code
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
```

## License

MIT License

Copyright (c) 2025 Kidus Alemayehu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Contributing

Feel free to open issues or submit pull requests for improvements.

## Contact

Created by [Kidus Alemayehu](https://github.com/KidusAlemayehu).
