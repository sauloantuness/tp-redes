package main
import "encoding/hex"
import "fmt"
import "io/ioutil"
import "net"

const (
    CONN_TYPE = "tcp"
    CONN_HOST = "localhost"
    CONN_PORT = "3333"
    CONN_LADDR = CONN_HOST + ":" + CONN_PORT
)

func main2() {
    d1 := []byte("hellogo")
    var d2 = hex.EncodeToString(d1)
    ioutil.WriteFile("/home/saulo/dev/tp-redes/go/dat", []byte(d2), 0644)
    dat, _ := ioutil.ReadFile("/home/saulo/dev/tp-redes/go/dat")
    var d3 = hex.Dump(dat)
    fmt.Print(d3)
}

func main() {
    getPDU()
}


func getPDU () []byte {
    ln, _ := net.Listen(CONN_TYPE, CONN_LADDR)
    conn, _ := ln.Accept()
    data := make([]byte, 1024)
    n, _ := conn.Read(data)
    data = data[:n]
    fmt.Println(data)
    conn.Close()
    return data
}

func sendPDU (data []byte) {
    conn, _ := net.Dial(CONN_TYPE, CONN_LADDR)
    conn.Write(data)
    conn.Close()
}