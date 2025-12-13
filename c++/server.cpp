#include <winsock2.h>           
#include <ws2tcpip.h>           
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>            
#include <cctype>
#include <string>

#pragma comment(lib, "Ws2_32.lib") 

static const int PORT = 54000;          // Puerto del servidor
static const int BUFSIZE = 1024;        // Tamaño del buffer

void handle_client(SOCKET client) {
    char buf[BUFSIZE];
    std::string msg, response;
    int bytes = 0;

    do {
        if ((bytes = recv(client, buf, BUFSIZE - 1, 0)) > 0) {  // Si recibimos un mensaje no vacio
            buf[bytes] = '\0';      // Añadimos final de carro
            msg = buf;
            // transforma a mayusculas la cadena
            std::transform(msg.begin(), msg.end(), msg.begin(), [](unsigned char c){ return std::toupper(c); });      
            if (msg != "QUIT") {              // si no envia 'QUIT'
                response = msg + "\n";
                send(client, response.c_str(), (int)response.size(), 0);    // envio mensaje en mayusculas
            }
        }
    } while (msg != "QUIT");

    closesocket(client);        // Cuando salga del do-while, finaliza conexión
    std::cout << "Client disconnected.\n";
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {   // Si no inicia correctamente salimos
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);      // Creación del socket
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};         // Estructura de socket TCP/IP 
    serverAddr.sin_family = AF_INET;      
    serverAddr.sin_port = htons(PORT);    
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Acepta cualquier conexión
    
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));    // Asociamos datos estructura al socket  
    
    listen(serverSocket, SOMAXCONN);   // Habilita el socket para recibir peticiones de conexión 

    std::cout << "Server listening in port " << PORT << "\n";

    std::vector<std::thread> threads;       // Vector de hilos
    while (true) {
        sockaddr_in clientAddr;     
        int clientSize = sizeof(clientAddr);
        // Acepta conexión y almacena el socket entrante  
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
        std::cout << "Client connected.\n";
        // lanzar hilo detached 
        threads.emplace_back(std::thread(handle_client, clientSocket));
        threads.back().detach();
    }

    closesocket(serverSocket);        // Cerramos el socket
    WSACleanup();
    return 0;
}