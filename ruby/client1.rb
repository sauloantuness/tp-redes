#client sends file to server
require 'socket'

CONN_TRANSP = 7893 #port to layer in go
CONN_NETWORK = 7892 #port to layer in c
HOST = '127.0.0.1'

sockc = TCPServer.new(HOST,CONN_TRANSP)
loop {
	#socket to layer in c
  Thread.start(sockc.accept) do |client|
  puts "Connected layer in c"

	#receveid message layer in c
	message = client.recv(3000)
	print "Message receveid layer in c: " + message + "\n"

	# socket to layer in go
	sock = TCPSocket.new(HOST, CONN_NETWORK)
  puts "Connected layer in go"

	#send message receveid layer in c to layer in go
	sock.write(message + "\n")
  puts "Send message layer in go:\n" +  message + "\n"

	#receveid messenge layer in go
	messr = sock.recv(3000)
	print "Message receveid layer in go: " +  messr.chomp + "\n"

	#send message receveid in go to layer in c
	client.write(messr + "\n" )
	#sock.close
	end
}

#http://www.backtrack-linux.org/forums/showthread.php?t=22070
