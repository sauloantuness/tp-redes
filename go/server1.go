package main

import "net"
import "fmt"

const (
    CONN_TYPE = "tcp"
    CONN_NETWORK = "localhost:7895"
    CONN_PHYSICAL = "localhost:7891"
)

func readPDU(conn net.Conn) []byte {
  pdu := make([]byte, 1024)
  n, _ := conn.Read(pdu)
  pdu = pdu[:n]
  return pdu
}

func main() {

  fmt.Println("Launching server...")
  // listen on all interfaces in go
  ln, _ := net.Listen(CONN_TYPE, CONN_PHYSICAL)
  // accept connection on port
  conn, _ := ln.Accept()

  for {

    // run loop forever (or until ctrl-c)
    // connectede layer in ruby
    connTransp, _ := net.Dial(CONN_TYPE, CONN_NETWORK)

    // will listen for message to process ending in newline (\n) in go
    //message, _ := bufio.NewReader(conn).ReadString('\n')
    message := readPDU(conn)
    // output message received
    fmt.Print("Message Received:\n", string(message) + "\n")
    // sample process for string received
    //  newmessage := strings.ToUpper(message)

    // send message received layer in go to layer in ruby
    connTransp.Write([]byte(message))

    //output message received to layer in ruby
  //  message1, _ := bufio.NewReader(connTransp).ReadString('\n')
    message1 := readPDU(connTransp)
    fmt.Print("Message Received from server in RUBY:\n", string(message1))

    //send message client go
    conn.Write([]byte(message1))
  }
}
