package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
)

func main() {
	// Se conecta al servidor
	conn, err := net.Dial("tcp", "localhost:50000")
	if err != nil {
		fmt.Println("Could not connect to server:", err)
		return
	}
	defer conn.Close()

	fmt.Println("Connected to Echo Server on port 50000")
	fmt.Println("Type 'quit' to exit")
	fmt.Println()

	// Crea readers para la entrada del usuario y la respuesta del servidor
	reader := bufio.NewReader(conn)
	scanner := bufio.NewScanner(os.Stdin)

	for {
		// Lee entrada del usuario
		fmt.Print("> ")
		if !scanner.Scan() {
			break
		}

		message := scanner.Text()
		if strings.TrimSpace(message) == "" {
			continue
		}

		// Envía mensaje al servidor
		_, err := fmt.Fprintf(conn, "%s\n", message)
		if err != nil {
			fmt.Println("Error sending message:", err)
			return
		}

		// Si enviamos "quit", salimos
		if strings.EqualFold(strings.TrimSpace(message), "quit") {
			fmt.Println("Closing connection...")
			return
		}

		// Lee respuesta del servidor
		response, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Error reading response:", err)
			return
		}

		fmt.Printf("Echo: %s\n", strings.TrimSpace(response))
	}
}
