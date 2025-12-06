import socket
import threading

HOST = "127.0.0.1"   
PORT = 5000

def manejar_cliente(conn, addr):
    print(f"[+] Nueva conexión desde {addr}")
    with conn:
        while True:
            data = conn.recv(1024)
            if not data:
                break
            # Devuelve el mismo mensaje
            conn.sendall(data)
    print(f"[-] Cliente desconectado {addr}")

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen()

    print(f"[SERVIDOR] Escuchando en {HOST}:{PORT}")

    while True:
        conn, addr = server.accept()
        hilo = threading.Thread(target=manejar_cliente, args=(conn, addr))
        hilo.start()

if __name__ == "__main__":
    main()
