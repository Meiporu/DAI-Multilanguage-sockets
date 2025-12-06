import socket

HOST = "127.0.0.1"
PORT = 5000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    print("Conectado al servidor.")

    while True:
        msg = input("Escribe un mensaje (o 'salir'): ")
        if msg.lower() == "salir":
            break
        
        s.sendall(msg.encode())
        data = s.recv(1024)

        print("Servidor respondió:", data.decode())
