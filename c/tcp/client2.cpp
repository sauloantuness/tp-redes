/*
    C ECHO client example using sockets
    http://www.binarytides.com/server-client-example-c-sockets-linux/
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<bits/stdc++.h>

#define CONN_APP 7894
#define CONN_NETWORK 7893
#define HOST "127.0.0.1"

using namespace std;

int sock_client, socket_desc , client_sock , c , read_size;
struct sockaddr_in server_client, server , client;
char message[3000] , server_reply[3000], client_message[3000], data[3000], segment[3000];

int createSocket();
string dec2bin ( unsigned int n );
void printSegment(char *segment);
void removeSegmento(char *segment, char *data);
void createSegmento(char *segmento);
string full_binary(string bin, int n);

int main(int argc , char *argv[])
{
      //Create socket to connect application layer
      socket_desc = createSocket();

      //Prepare the sockaddr_in structure
      server.sin_family = AF_INET;
      server.sin_addr.s_addr = INADDR_ANY;
      server.sin_port = htons( CONN_APP );

      //Bind
      if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
      {
          //print the error message
          perror("bind failed. Error");
          return 1;
      }
      puts("bind done");

      //Listen
      listen(socket_desc , 3);

      //Accept and incoming connection
      puts("Waiting for incoming connections...");
      c = sizeof(struct sockaddr_in);

      //accept connection from an incoming client
      client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
      if (client_sock < 0)
      {
          perror("accept failed");
          return 1;
      }
      puts("Connection accepted");

    //Create socket layer network
    sock_client = createSocket();

    server_client.sin_addr.s_addr = inet_addr(HOST);
    server_client.sin_family = AF_INET;
    server_client.sin_port = htons( CONN_NETWORK );

    //Connect at the network layer
    if (connect(sock_client , (struct sockaddr *)&server_client , sizeof(server_client)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //Receive a message layer application
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
          //create segment
          createSegmento(segment);

          //insert message in the segment end
          strcat(segment, client_message);

          //print segment
          printSegment(segment);

          //Send the messsage to layer network
          if( send(sock_client ,segment , strlen(segment) , 0) < 0)
          {
              puts("Send failed");
              return 1;
          }

          //Receive a reply layer network
          if( recv(sock_client , server_reply , 2000 , 0) < 0)
          {
              puts("recv failed");
              break;
          }

          puts("Server reply :");
          //print message
          printSegment(server_reply);
          removeSegmento(server_reply, data);

          //Send the message back to browser
          write(client_sock , data , strlen(data));
          
          memset(segment,0,3000);
          memset(data,0,3000);
          memset(client_message,0,2000);
          memset(server_reply,0,2000);

    }
    close(sock_client);
    return 0;
}

int createSocket(){
  int sock_client = socket(AF_INET , SOCK_STREAM , 0);
  if (sock_client == -1)
  {
      printf("Could not create socket");
  }
  puts("Socket created");
  return sock_client;
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
  cout << "            Data:\n" << s.substr(192,s.length()) << endl;
  cout << endl;
}
