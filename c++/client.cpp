#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

static const char* SERVER_IP = "127.0.0.1";     // Dirreción del servidor
static const int SERVER_PORT = 54000;       // Puerto del servidor
static const int BUFSIZE = 1024;        

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {     // Si no inicia correctamente salimos
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    // Creamos socket

    sockaddr_in serverAddr{};                    // Estructura de socket TCP/IP
    serverAddr.sin_family = AF_INET;                // IPv4
    serverAddr.sin_port = htons(SERVER_PORT);           // Puerto del servidor
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);    // IPv4 del servidor

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));      // Solicita una conexión al servidor

    std::string line;
    char buf[BUFSIZE];
    std::cout << "EchoServer in C++.\nWrite messages (introduce 'quit' to exit):\n";
    while (std::getline(std::cin, line)) {      // Mientras se escriba
        line += "\n";               // enviar con salto de linea para que el servidor lo vea como linea
        send(sock, line.c_str(), (int)line.size(), 0);
        int bytes = recv(sock, buf, BUFSIZE - 1, 0);    // Respuesta servidor
        if (bytes <= 0) break;          // Si no hay respuesta, finalizamos conexión
        buf[bytes] = '\0';              // En caso contrario, añadimos final de carro 
        std::cout << "Response: " << buf;   // Mostramos respuesta
    }

    closesocket(sock);      // Cerramos el socket
    WSACleanup();
    return 0;
}