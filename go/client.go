package main

import "net"
import "fmt"

const (
    CONN_TYPE = "tcp"
    CONN_NETWORK = "localhost:9003"
    CONN_PHYSICAL = "localhost:9004"
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

func main () {
  // Create a socket to listen the superior layer
  ln, _ := net.Listen(CONN_TYPE, CONN_NETWORK)
    for {
      connNetwork, _ := ln.Accept()
      packet := readPDU(connNetwork)
      fmt.Printf("< packet: % x\n", packet)
      frame := createFrame(packet)

      // Send Physical PDU
      connPhysical, _ := net.Dial(CONN_TYPE, CONN_PHYSICAL)
      fmt.Printf("> frame: % x\n", frame)
      printFrame(frame)
      connPhysical.Write(frame)

      // Receive Physical PDU
      frame = readPDU(connPhysical)
      fmt.Printf("< frame: % x\n", frame)
      printFrame(frame)
      packet = removeFrame(frame)

      // Send Network PDU
      fmt.Printf("> packet: % s\n", packet)
      connNetwork.Write(packet)
      connNetwork.Close()
      fmt.Println()
    }
}
