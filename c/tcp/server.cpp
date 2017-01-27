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

void createSegmento(char *segmento){
  char seg[1024];
  strcat(seg,"xxxxxxxxxxxxxxxx");// port source - 16
  strcat(seg,"yyyyyyyyyyyyyyyy");// port destination - 16
  strcat(seg,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");// sequence number - 32
  strcat(seg,"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");// acknowlegement - 32
  strcat(seg,"ssss");// tam - 4
  strcat(seg,"wwww");// reserve - 4
  strcat(seg,"rrrr");// flags - 4
  strcat(seg,"dddddddddddddddddddd"); // window - 20
  strcat(seg,"xxxxxxxxxxxxxxxx");// checksum - 16
  strcat(seg,"yyyyyyyyyyyyyyyy");// urgent pointer - 16
  strcat(seg,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");// options - 32
  strcat(segmento, seg);
}

void removeSegmento(char *segment, char *data){
  int length = strlen(segment), i=0, j=0;
  for(i=192, j=0;i<length;i++,j++){
      data[j] = segment[i];
  }
}

void printSegment(char *segment){
  string s = segment;
  cout << "Port Source: " << s.substr(0,16) << endl;
  cout << "Port destination: " << s.substr(16,16) << endl;
  cout << "Sequence number: " << s.substr(32,32) << endl;
  cout << "Acknowlegement: " << s.substr(64,32) << endl;
  cout << "Tam: " << s.substr(96,4) << endl;
  cout << "Reserve: " << s.substr(100,4) << endl;
  cout << "Flags: " << s.substr(104,4) << endl;
  cout << "Window: " << s.substr(108,20) << endl;
  cout << "Checksum: " << s.substr(128,16) << endl;
  cout << "Urgent Pointer: " << s.substr(144,16) << endl;
  cout << "Options: " << s.substr(160,32) << endl;
  cout << "Data: " << s.substr(192,s.length()) << endl;

  cout << "family: " <<server.sin_family << endl;
  cout << "port: " <<server.sin_port << endl;
  printf("Port: %d\n",server.sin_port);
  printf("Addr: %d\n",server.sin_addr.s_addr);
}

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
