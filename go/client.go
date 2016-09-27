package main

import "net"
import "fmt"
import "bufio"
import "os"

const (
    CONN_TYPE = "tcp"
    CONN_HOST = "localhost"
    CONN_PORT = "3333"
)

func main() {
  conn, _ := net.Dial(CONN_TYPE, CONN_HOST + ":" + CONN_PORT)
  
  for { 
    fmt.Print("Text to send: ")
    request, _ := bufio.NewReader(os.Stdin).ReadString('\n')
    
    fmt.Fprintf(conn, request + "\n")
    
    response, _ := bufio.NewReader(conn).ReadString('\n')
    fmt.Print("Message from server: " + response)
  }
}