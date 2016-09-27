package main

import "net"
import "fmt"
import "bufio"
import "strings" // only needed below for sample processing

const (
    CONN_TYPE = "tcp"
    CONN_HOST = "localhost"
    CONN_PORT = "3333"
)

func main() {

  fmt.Println("Launching server...")

  ln, _ := net.Listen(CONN_TYPE, CONN_HOST + ":" + CONN_PORT)
  conn, _ := ln.Accept()

  for {
    request, _ := bufio.NewReader(conn).ReadString('\n')
    fmt.Print("Message Received:", request)
    response := strings.ToUpper(request)
    fmt.Fprintf(conn, response + "\n")
  }
}