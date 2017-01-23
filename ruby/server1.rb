#server receive a file from client
require 'socket'
CONN_TRANSP = 7896
CONN_NETWORK = 7895
HOST = '127.0.0.1'

server = TCPServer.new(HOST,CONN_NETWORK) #socket server

puts "Listen..."
loop {
    Thread.start(server.accept) do |client|
    puts "Connected layer in c"
    #receveid message socket in go
  	data = client.recv(3000)
    puts "Messenge receveid:\n"  + data + "\n"


    #Connected layer in c
    sock = TCPSocket.new(HOST, CONN_TRANSP)
    puts "Connected layer in go"

    #send message receveid to socket in c
  	sock.write(data + "\n")
    puts "Send message layer in c:\n" +  data + "\n"

    #receveid socket in c
  	messr = sock.recv(3000)
  	print "Message receveid: " +  messr.chomp + "\n"

    #send message layer in c to socket in go
  	client.write(messr)
    end

}
#http://www.backtrack-linux.org/forums/showthread.php?t=22070
