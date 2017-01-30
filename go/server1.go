package main

import "net"
import "fmt"


const (
    CONN_TYPE = "tcp"
    CONN_NETWORK = "localhost:7895"
    CONN_PHYSICAL = ":7891"
)

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
  DestinationMAC  := []byte{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}
  SourceMAC       := []byte{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}
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

  fmt.Println("Launching server...")
  // connectede layer in ruby
  connNetwork, _ := net.Dial(CONN_TYPE, CONN_NETWORK)

  // listen on all interfaces in go
  ln, _ := net.Listen(CONN_TYPE, CONN_PHYSICAL)

  for {  // run loop forever (or until ctrl-c)

    // accept connection on port
    connPhysical, _ := ln.Accept()

    // will listen for message to process ending in newline (\n) in go
    frame := readPDU(connPhysical)
    // output message received
    //fmt.Print("Message Received:\n", string(message) + "\n")
    fmt.Printf("Message Received:\n< frame:\n")
    printFrame(frame)
    packet := removeFrame(frame)

    //print packet
    fmt.Printf("> packet: \n% s\n", packet)
    // send message received layer in go to layer in ruby
    connNetwork.Write([]byte(packet))

    //output message received to layer in ruby
    //  message1, _ := bufio.NewReader(connNetwork).ReadString('\n')
    packet = readPDU(connNetwork)
    //fmt.Print("Message Received from server in RUBY:\n", string(message1))
    fmt.Printf("< packet: \n% s\n", packet)
    frame = createFrame(packet)

    fmt.Printf("> frame:\n")
    printFrame(frame)
    fmt.Println()
    //send message client go
    connPhysical.Write([]byte(frame))
    connPhysical.Close()
  }
}
