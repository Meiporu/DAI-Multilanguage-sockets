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
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};                    // Estructura de socket TCP/IP
    serverAddr.sin_family = AF_INET;                
    serverAddr.sin_port = htons(SERVER_PORT);           
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);    

    // Solicita una conexión al servidor
    if(connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Unable to connect to server\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }      

    std::string line;
    char buf[BUFSIZE];
    std::cout << "EchoServer in C++.\nWrite messages ('quit' to exit)\n";
    while (std::getline(std::cin, line)) {      
        send(sock, line.c_str(), (int)line.size(), 0);
        int bytes = recv(sock, buf, BUFSIZE - 1, 0);    // Respuesta servidor
        if (bytes <= 0) break;          // Si no hay respuesta, finalizamos conexión
        buf[bytes] = '\0';              // En caso contrario, mostramos respuesta 
        std::cout << "Response: " << buf;   
    }

    closesocket(sock);      // Cerramos el socket
    WSACleanup();
    return 0;
}