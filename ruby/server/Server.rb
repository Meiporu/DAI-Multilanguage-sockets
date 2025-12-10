=begin
    Echo Server Implementation
=end

#Carga la libreria de sockets
require "socket"

#Configuración para usar en el Socket
PORT = 50000 


#Definicion de una funcion para multihilo
def threadFunc(clientSocket)
        puts "Se ha abierto una conexion"
    begin
        while true
            recibido = clientSocket.gets.chomp
             if recibido == "quit"
                #Parar el bucle 
                break
            end # End del if
            clientSocket.puts(recibido.upcase)
        end # Fin del bucle
    rescue StandardError  => e
        puts "Se ha producido un error en el hilo: #{e.message}"
    ensure
        puts "Se ha cerrado una conexion"
        clientSocket.close
    end # Fin bloque de codigo
end # Fin funcion


#Definicion de una funcion por comodidad
def startServer
    # Se abre el socket con open y se usa un bloque de codigo con open para cerrar automaticamente el socket
    # Similar a try-with-resources
    TCPServer.open(PORT) do |serverSocket|
        while true
            #Acepta conexiones
            cliente = serverSocket.accept
            #Envia a un hilo para atender multiples clientes
            Thread.new {threadFunc(cliente)}

        end # Fin bucle
    rescue  StandardError  => e
        puts "Se ha producido un error : #{e.message}"
    end #Fin del bloque de codigo
end # Fin de la funcion



#Comienzo del programa

puts "Iniciando Echo Server"
#Ejecución de la funcion
startServer
puts "Se ha finalizado Echo Server"
