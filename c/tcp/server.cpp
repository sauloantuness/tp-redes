/*
    C socket server example
    http://www.binarytides.com/server-client-example-c-sockets-linux/

*/

#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<bits/stdc++.h>

using namespace std;

#define CONN_APP 7897
#define CONN_NETWORK 7896
#define HOST "127.0.0.1"
//http://www.mfa.unc.br/info/carlosrafael/rco/aula17-1.pdf
int socket_desc , client_sock , c , read_size;
struct sockaddr_in server , client;
char client_message[2000], segment[3000], data[3000];

int createSocket();
string dec2bin ( unsigned int n );
void printSegment(char *segment);
void removeSegmento(char *segment, char *data);
void createSegmento(char *segmento);
string full_binary(string bin, int n);


int main(int argc , char *argv[])
{
  //Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);
  if (socket_desc == -1)
  {
      printf("Could not create socket");
  }
  puts("Socket created");

  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons( CONN_APP );
    char client_message[2000], segment[3000], data[3000];
    createSegmento(segment);
    printSegment(segment);



    return 0;
}


string dec2bin ( unsigned int n ) {
  if ( n == 0) return "0" ;
  if ( n == 1) return "1" ;
  if ( n % 2 == 0) return dec2bin ( n / 2) + "0" ;
  return dec2bin ( n / 2) + "1" ;
}

string full_binary(string bin, int n){
  int x = n - bin.length();
  string aux;
  while(x--){
    aux.append("0");
  }
  aux.append(bin);
  return aux;
}
void createSegmento(char *segmento){
  string seg;
  seg.append(full_binary(dec2bin(CONN_APP),16));// port source - 16
  seg.append(full_binary(dec2bin(CONN_NETWORK),16));// port destination - 16
  seg.append(full_binary("0",32));// sequence number - 32
  seg.append(full_binary("0",32));// acknowlegement - 32
  seg.append(full_binary("0",4));// tam - 4
  seg.append(full_binary("0",4));// reserve - 4
  seg.append(full_binary("0",4));// flags - 4
  seg.append(full_binary("0",20)); // window - 20
  seg.append(full_binary("0",16));// checksum - 16
  seg.append(full_binary("0",16));// urgent pointer - 16
  seg.append(full_binary("0",32));// options - 32
  strcat(segmento, seg.c_str());
}

void removeSegmento(char *segment, char *data){
  int length = strlen(segment), i=0, j=0;
  for(i=192, j=0;i<length;i++,j++){
      data[j] = segment[i];
  }
}

void printSegment(char *segment){
  string s = segment;
  cout << endl;
  cout << "     Port Source: " << s.substr(0,16) << endl;
  cout << "Port destination: " << s.substr(16,16) << endl;
  cout << " Sequence number: " << s.substr(32,32) << endl;
  cout << "  Acknowlegement: " << s.substr(64,32) << endl;
  cout << "             Tam: " << s.substr(96,4) << endl;
  cout << "         Reserve: " << s.substr(100,4) << endl;
  cout << "           Flags: " << s.substr(104,4) << endl;
  cout << "          Window: " << s.substr(108,20) << endl;
  cout << "        Checksum: " << s.substr(128,16) << endl;
  cout << "  Urgent Pointer: " << s.substr(144,16) << endl;
  cout << "         Options: " << s.substr(160,32) << endl;
  cout << "            Data: " << s.substr(192,s.length()) << endl;
  cout << endl;
}
