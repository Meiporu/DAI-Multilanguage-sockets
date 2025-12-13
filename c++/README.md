# Sockets en C++

Guia de ayuda para explicar el código, quizás hasta sirva para poner en la presentación algo:

**Se usará la API de sockets en C para Windows Winsock2**

- **Integración nativa con Windows:** es la API oficial del sistema, garantizando compatibilidad, soporte y acceso directo a las funciones del SO.
- **Control de bajo nivel:** ofrece acceso detallado a configuraciones del socket, opciones avanzadas y comportamiento específico del sistema.



- Estrucutras necesarias:

```
// Estructura genérica que usan las APIs de sockets
struct sockaddr
{
    unsigned short sa_family; // Indica la familia a usar (AF_INET/AF_INET6) 
    char sa_data[14]; // Dirección de protocolo. No se usa actualmente
};

// Estructura enfocada a IPv4 con TCP/IP, equivalente a sockaddr pero con los siguientes campos:
struct sockaddr_in
{
    short int sin_family; // PF_INET
    unsigned short sin_port; // Numero de puerto. Se representa en orden de bytes de red (big_endyan)
    struct in_addr sin_addr; // Estructura in_addr que contiene la IPv4 en binario
    unsigned char sin_zero[8]; // Relleno.
};

// Estructura que contiene la dirección IPv4 en 32 bits
struct in_addr
{
    unsigned long s_addr; // IP en 4 bytes.
}; 
```

### Librerias

- **winsock2.h**: API SOCKETS. Contiene definiciones de las funciones y constantes necesarias para trabajar con sockets.
- **ws2tcpip.h**: Utilidades IPv4/IPv6 y funciones modernas(TCP/IP)
- `#pragma comment(lib, "Ws2_32.lib")`: Libreria 

### Constantes

- **PORT:** Constante que indica el puerto a usar.
- **BUFSIZE:** Constante que simboliza el tamaño de los buffers I/O.
- **AF\_INET:** Representa que usaremos IPv4.
- **SOCK\_STREAM:** Representa un socket Stream.
- **IPPROTO\_TCP:** Protocolo a usar junto con Stream para indicar conexion TCP.
- **INVALID\_SOCKET:** Constante que devuelve funciones como bind() o listen() cuando ocurre un error en el sockey.
- **SOMAXCONN:** Constante por defecto que nos define la longitud de la cola de espera de sockets.
- **SOCKET\_ERROR:** Constante que indica que ocurrio un error

### Funciones

- **WSDATA:** Estructura que la librería Winsock rellena con información sobre la implementación de sockets que se va a usar. (DLL Ws2\_32.dll)
- **WSAStartup(wVersionRequested, \&wsaData):** Prepara recursos y carga ua implementación WinSock. Si se realizo correctmanet devuelve 0
  - wVersionRequested: Verión a usar (2.2) de tipo WORD
  - \&wsaData: WSDATA

- **SOCKET socket(int domain, int type, int protocol):** devuelve el descriptor del socket, reservando recursos del sistema. Si no se crea correctamente devuelve INVALID\_SOCKET. Parámetros:
  - *domain*: Dominio donde se hace la conexión. Sera AF\_INET (IPv4) 
  - *type*: Tipo de socket a usar. En este caso orientados a conexión (SOCK\_STREAM)
  - *protocol*: Protocolo de transporte. IPPROTO\_TCP especifica que usaremos TCP. Se puede usar 0 en su lugar y elige el sistema

- **struct sockaddr\_in:** Estructura TCP/IP, equivalente a la estructura genérica sockaddr pero con los siguientes campos:
  - *sin\_family:* Sera AF\_INET (IPv4). Protocolo IP a usar
  - *sin\_port:* Representa el puerto donde escucha. Se espera valor en bytes de red (big-endian) - htons() - conversión
  - *sin\_addr.s\_addr:* IP para acepta conexioes. Formato de struct in\_addr con campo s\_addque es usigned long de 4 bytes (representa IP)

- **int bind(SOCKET sockfd, struct sockaddr *my_addr, int addrlen):*** Asocia al Socket que deseemos los parámetros (IP+Puerto) necesarios
  - *sockfd:* Socket que queremos rellenar.
  - *\*my_addr:* Estrucutra con la información IP+Puerto del socket
  - *addrlen:* Longitud de la estructura (sizeof(my_addr))

- **int listen(SOCKET sockfd, int backlog):** Habilita al socket a recibir peticiones de conexión, donde:
  
  - sockfd: Socket que servira de escucha.
  - entero que indica el tamaño de la cola de conexiones pendientes. Si trae SOMAXCONN nos indica el numero maximo razonable del SO.

- **SOCKET accept(SOCKET sockfd, struct sockaddr \*addr, socklen_t \*addrlen):** Acepta una conexión socket que le llego al servidor, donde:

  - sockfd: Es el socket de escucha.
  - \*addr: Puntero a la estrucutra donde se guardará información sobre el socket entrante.
  - \*addrlen: Puntero a una estructura que indica el tamaño de donde se guardará

  
- **int connect(SOCKET sockfd, const struct sockaddr \*serv_addr, socklen_t addrlen):** Solicita una conexión TCP, donde:

  - sockfd: Socket que se enviará para conectar.
  - \*serv_addr: Puntero a una estructura que contiene la IP y puerto del servidor.
  - addrlen: Tamaño de la estructura \*serv_addr.


## Comandos:
### Compilar:

  - `g++ -std=c++17 server.cpp -o server.exe -lws2_32`
  - `g++ -std=c++17 client.cpp -o client.exe -lws2_32`

### Ejecutar:

  - `server.exe`
  - `client.exe`

## Bibliografía usada:

- *Universidad de Cantabria/ Manual de sockets en C* - https://ocw.unican.es/pluginfile.php/2343/course/section/2300/PIR-Practica4\_ManualSocketsC.pdf 
- *API winsock2.h* - https://learn.microsoft.com/es-es/windows/win32/api/winsock2/

> Guía hecha por Miguel Ángel Seara Losada 