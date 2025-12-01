package main

import (
	"bufio"
	"fmt"
	"net"
	"strings"
)

func main() {
	// Escucha en el puerto 50000
	ln, err := net.Listen("tcp", ":50000")
	if err != nil {
		fmt.Println("Server socket could not be created:", err)
		return
	}
	defer ln.Close()

	fmt.Println("Echo Server listening on port 50000...")

	// Acepta conexiones entrantes en un bucle
	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println(err)
			continue
		}

		fmt.Println("Client connected:", conn.RemoteAddr())

		// Maneja la conexión en una nueva goroutine (similar a hilos en Java)
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	// Cierra la conexión cuando hayamos terminado
	defer conn.Close()
	defer fmt.Println("Client disconnected:", conn.RemoteAddr())

	// Crea un reader y writer para manejar datos línea por línea
	reader := bufio.NewReader(conn)
	writer := bufio.NewWriter(conn)

	// Lee mensajes en un bucle hasta recibir "quit"
	for {
		// Lee mensaje del cliente (hasta encontrar un salto de línea)
		message, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Error reading:", err)
			return
		}

		// Elimina espacios en blanco y saltos de línea
		message = strings.TrimSpace(message)

		// Si el mensaje es "quit", cierra la conexión
		if strings.EqualFold(message, "quit") {
			fmt.Printf("Client %s sent quit. Closing connection.\n", conn.RemoteAddr())
			return
		}

		// Convierte a mayúsculas y envía de vuelta al cliente
		response := strings.ToUpper(message) + "\n"
		_, err = writer.WriteString(response)
		if err != nil {
			fmt.Println("Error writing:", err)
			return
		}

		err = writer.Flush()
		if err != nil {
			fmt.Println(err)
			return
		}
	}
}
