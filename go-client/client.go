package main

import (
	"crypto/tls"
	"fmt"
	"log"
	"sync"
	"time"
)

// Testing Client in Go for concurrent requests testing purposes

const (
	// Number of requests to make
	serverAddr    = "127.0.0.1:8050"
	numOfClients  = 5
	numOfRequests = 20
	duration      = 500 * time.Millisecond
)

func main() {
	var wg sync.WaitGroup
	wg.Add(numOfClients)

	for i := 0; i < numOfClients; i++ {
		go func(clientID int) {
			runClients(clientID)
			wg.Done()
		}(i)
	}

	wg.Wait()
	log.Println("All clients have finished")
}

func runClients(clientID int) {
	tlsConfig := &tls.Config{
		InsecureSkipVerify: true,
	}

	conn, err := tls.Dial("tcp", serverAddr, tlsConfig)
	if err != nil {
		log.Fatalf("Error connecting to server: %v", err)
		return
	}
	defer conn.Close()

	log.Printf("Client %d connected to server", clientID)

	for i := 0; i < numOfRequests; i++ {
		message := fmt.Sprintf("Client %d: Message %d", clientID, i)

		_, err := conn.Write([]byte(message))
		if err != nil {
			log.Fatalf("Error sending message: %v", err)
			return
		}

		buffer := make([]byte, 1024)
		n, err := conn.Read(buffer)
		if err != nil {
			log.Printf("Client %d: Failed to read response: %v\n", clientID, err)
			return
		}
		log.Printf("Client %d: Received response: %s\n", clientID, string(buffer[:n]))

		time.Sleep(duration)
	}
}
