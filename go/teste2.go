package main

import "net"

const (
    CONN_TYPE = "tcp"
    CONN_HOST = "localhost"
    CONN_PORT = "3333"
)

func main() {
    conn, _ := net.Dial(CONN_TYPE, CONN_HOST + ":" + CONN_PORT)
    data := []byte("Teste")
    conn.Write(data)
}