package main

import "net"
import "fmt"
import "io/ioutil"

const (
    CONN_TYPE = "tcp"
    CONN_NETWORK_HOST = "localhost"
    CONN_NETWORK_PORT = "3335"
    CONN_NETWORK = CONN_NETWORK_HOST + ":" + CONN_NETWORK_PORT
)

func readPDU(conn net.Conn) []byte {
  pdu := make([]byte, 10)
  n, _ := conn.Read(pdu)
  pdu = pdu[:n]
  return pdu
}

func main () {
  // Create a socket to listen the physical layer
  ln, _ := net.Listen(CONN_TYPE, CONN_NETWORK)
    for {
      connNetwork, _ := ln.Accept()
      packet := readPDU(connNetwork)
      fmt.Println("< packet: ", string(packet))
      packet, _ = ioutil.ReadFile("/home/saulo/dev/tp-redes/go/packet")
      fmt.Println("> packet: ", string(packet))
      connNetwork.Write(packet)
      connNetwork.Close()
    }
}