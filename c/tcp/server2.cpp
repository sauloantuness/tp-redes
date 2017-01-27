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

#define CONN_APP 7897
#define CONN_NETWORK 7896
#define HOST "127.0.0.1"

using namespace std;

int sock_client, socket_desc , client_sock , c , read_size;
struct sockaddr_in server_client, server , client;
char message[1000] , server_reply[2000],client_message[2000];

int createSocket(){
  //Create socket layer network
  int sock_client = socket(AF_INET , SOCK_STREAM , 0);
  if (sock_client == -1)
  {
      printf("Could not create socket");
  }
  puts("Socket created");
  return sock_client;
}

int main(int argc , char *argv[])
{
  //Create socket client
  sock_client = createSocket();

  server_client.sin_addr.s_addr = inet_addr(HOST);
  server_client.sin_family = AF_INET;
  server_client.sin_port = htons( CONN_APP );

  //Connect to remote server
  if (connect(sock_client , (struct sockaddr *)&server_client , sizeof(server_client)) < 0)
  {
      perror("connect failed. Error");
      return 1;
  }

  puts("Connected\n");

    //Create socket
    socket_desc = createSocket();

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( CONN_NETWORK );

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
        //Send some data
        if( send(sock_client , client_message , strlen(client_message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if( recv(sock_client , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        puts("Server reply :");
        //  puts(server_reply);
        printf("%s\n",server_reply );

        //Send the message back to client
        write(client_sock , server_reply , strlen(server_reply));
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
