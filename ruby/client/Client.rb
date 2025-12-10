=begin
    Echo Client Implementation
=end

#Carga la libreria de sockets
require "socket"

#Configuración para usar en el Socket
HOSTNAME = "localhost"
PORT = 50000 


#Definicion de una funcion por comodidad
def funcionClient
    # Se abre el socket con open y se usa un bloque de codigo con open para cerrar automaticamente el socket
    # Similar a try-with-resources
    TCPSocket.open(HOSTNAME,PORT) do |socket|
        #Mientras el usuario no detenga el programa, continua
        puts "Escribe un mensaje para procesarlo, o quit para salir"
        while true        
            #Leer por teclado un string
            leido = gets.chomp() #chomp para eliminar el /n final
            puts "Se ha leido: #{leido}"
            socket.puts(leido) #Escribe en el socket
            #si es quit parar
            if leido == "quit"
                #Parar el bucle 
                puts "Deteniendo Echo Client..."
                break
            else
                recibido = socket.gets #Lee del socket
                puts "ECHO : #{recibido.chomp}"
            end # End del if-else            
        end # End del while
    rescue  Errno::ECONNREFUSED => e #Error al conectarse
        puts "No se ha podido establecer la conexion : #{e.message}"
    rescue  StandardError  => e
        puts "Se ha producido un error : #{e.message}"
    end # End del bloque de codigo

end #Fin de la función


#Comienzo del programa

puts "Iniciando Echo Client"
#Ejecución de la funcion
funcionClient
puts "Se ha finalizado Echo Client"