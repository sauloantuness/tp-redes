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

struct sockaddr_in server , client;


void createSegmento(char *segmento){
  char seg[1024];
  strcat(seg,"xxxxxxxxxxxxxxxx");// port source - 16
  strcat(seg,"yyyyyyyyyyyyyyyy");// port destination - 16
  strcat(seg,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");// sequence number - 32
  strcat(seg,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");// acknowlegement - 32
  strcat(seg,"ssss");// tam - 4
  strcat(seg,"wwww");// reserve - 4
  strcat(seg,"rrrr");// flags - 4
  strcat(seg,"dddddddddddddddddddd"); // window - 20
  strcat(seg,"xxxxxxxxxxxxxxxx");// checksum - 16
  strcat(seg,"yyyyyyyyyyyyyyyy");// urgent pointer - 16
  strcat(seg,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");// options - 32
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
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    char client_message[2000], segment[3000], data[3000];

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

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

    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        printSegment(client_message);
        removeSegmento(client_message, data);
        printf("%s\n", data);
        printf("Your message: %s\n", client_message );

        createSegmento(segment);
        strcat(segment, "Retorno");
        printf("%s\n", segment);
        //Send the message back to client
        write(client_sock , segment , strlen(segment));
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}
