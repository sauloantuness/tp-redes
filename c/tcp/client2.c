/*
    C ECHO client example using sockets
    http://www.binarytides.com/server-client-example-c-sockets-linux/
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#define CONN_APP 7894
#define CONN_NETWORK 7893
#define HOST "127.0.0.1"

int sock_client, socket_desc , client_sock , c , read_size;
struct sockaddr_in server_client, server , client;
char message[3000] , server_reply[3000],client_message[3000];

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

    /*-----*/

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


    //Receive a message layer application
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
          //Send the messsage to layer network
          printf("Message receveid browser:\n %s",client_message );
          printf("\n");
          if( send(sock_client , client_message , strlen(client_message) , 0) < 0)
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
          //  puts(server_reply);
          printf("%s\n",server_reply );

          //Send the message back to browser
          write(client_sock , server_reply , strlen(server_reply));

    }
    close(sock_client);
    return 0;
}
