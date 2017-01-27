package main

import "net"
import "fmt"
import "os"

const (
    CONN_TYPE = "tcp"
    //CONN_NETWORK = "localhost:7892"
    //CONN_PHYSICAL = "localhost:7891"
)

func checkError(err error) {
    if err != nil {
        fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
        os.Exit(1)
    }
}

func printFrame(Frame []byte) {
  Preamble        := Frame[:8]
  DestinationMAC  := Frame[8:14]
  SourceMAC       := Frame[14:20]
  Ethertype       := Frame[20:22]
  CRC             := Frame[len(Frame) - 4:]
  Data            := Frame[22:len(Frame) - 4]

  fmt.Println()
  fmt.Printf("      Preamble: % x\n", Preamble)
  fmt.Printf("DestinationMAC: % x\n", DestinationMAC)
  fmt.Printf("     SourceMAC: % x\n", SourceMAC)
  fmt.Printf("     Ethertype: % x\n", Ethertype)
  fmt.Printf("          Data: % x\n", Data)
  fmt.Printf("           CRC: % x\n", CRC)
  fmt.Println()
}

func createFrame(Data []byte) []byte {
  var Frame []byte
  Preamble        := []byte{0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}
  DestinationMAC  := []byte{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}
  SourceMAC       := []byte{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}
  Ethertype       := []byte{0x00, 0xff}
  CRC             := []byte{0x01, 0x02, 0x03, 0x04}

  Frame = append(Frame, Preamble...)
  Frame = append(Frame, DestinationMAC...)
  Frame = append(Frame, SourceMAC...)
  Frame = append(Frame, Ethertype...)
  Frame = append(Frame, Data...)
  Frame = append(Frame, CRC...)

  return Frame
}

func removeFrame(Frame []byte) []byte {
  Data := Frame[22:len(Frame) - 4]
  return Data
}

func readPDU(conn net.Conn) []byte {
  pdu := make([]byte, 1024)
  n, _ := conn.Read(pdu)
  pdu = pdu[:n]
  return pdu
}

func main() {

  conn_network := "127.0.0.1:7892"
  conn_physical := "127.0.0.1:7891"
  CONN_NETWORK, err := net.ResolveTCPAddr("tcp4", conn_network)
  checkError(err)
  // connect layer in ruby
  ln, _ := net.ListenTCP(CONN_TYPE, CONN_NETWORK)

  for {
    CONN_PHYSICAL, err := net.ResolveTCPAddr("tcp4", conn_physical)
    checkError(err)
    // connect to this socket in go
    connPhysical, _ := net.DialTCP(CONN_TYPE, nil,CONN_PHYSICAL)//port socket go

    // accept connection on port
    connNetwork, _ := ln.Accept()
    fmt.Print("Connected layer in go\n")
    // will listen for message to process ending in newline (\n) layer in rubby
    //  message, _ := bufio.NewReader(connNetwork).ReadString('\n')
    packet := readPDU(connNetwork)

    // output message received
    fmt.Print("Message Received layer in rubby:\n", string(packet) + "\n")

    //create frame
    frame := createFrame(packet)
    //fmt.Printf("> frame:\n % x\n", frame)
    fmt.Printf("> frame:\n")
    printFrame(frame)

    // send message layer in ruby to socket in go
    connPhysical.Write([]byte(frame))

    // listen for reply in go
    // message1, _ := bufio.NewReader(connPhysical).ReadString('\n')
    frame = readPDU(connPhysical)
    //fmt.Printf("< frame:\n % x\n", frame)
    fmt.Printf("< frame:\n")
    printFrame(frame)
    packet = removeFrame(frame)
    //fmt.Print("Message from server:\n", string(packet1))

    // send new message back to client in RUBBY
    fmt.Printf("> packet:\n % s\n", packet)
    connNetwork.Write([]byte(packet))
    fmt.Println()
  }
}
