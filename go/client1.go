package main

import "net"
import "fmt"

const (
    CONN_TYPE = "tcp"
    CONN_NETWORK = "localhost:7892"
    CONN_PHYSICAL = "localhost:7891"
)

func readPDU(conn net.Conn) []byte {
  pdu := make([]byte, 1024)
  n, _ := conn.Read(pdu)
  pdu = pdu[:n]
  return pdu
}

func main() {
  conn, _ := net.Dial(CONN_TYPE, CONN_PHYSICAL)//port socket go

  // connect layer in ruby
  ln, _ := net.Listen(CONN_TYPE, CONN_NETWORK)


  for {  // connect to this socket in go

    // accept connection on port
    connTransp, _ := ln.Accept()
    fmt.Print("Connected layer in go\n")
    // will listen for message to process ending in newline (\n) layer in rubby
  //  message, _ := bufio.NewReader(connTransp).ReadString('\n')

  message := readPDU(connTransp)
    // output message received
    fmt.Print("Message Received layer in rubby:\n", string(message) + "\n")

    // send message layer in ruby to socket in go
    conn.Write([]byte(message))

    // listen for reply in go
  //  message1, _ := bufio.NewReader(conn).ReadString('\n')
    message1 := readPDU(conn)
    fmt.Print("Message from server:\n", string(message1))

    // send new message back to client in RUBBY
    connTransp.Write([]byte(message1))
  }
}
