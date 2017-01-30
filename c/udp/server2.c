/************* UDP SERVER CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define CONN_APP 7897
#define CONN_NETWORK 7896
#define HOST "127.0.0.1"

int socket_desc, nBytes, sock_client, portNum;
char buffer[1024], message[3000] , server_reply[3000], client_message[3000], data[3000], segment[3000];
struct sockaddr_in server, server_client;
struct sockaddr_storage serverStorage;
socklen_t addr_size, addr_size_client;

int main(){

    /*Create UDP socket application layer*/
    sock_client = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    server_client.sin_family = AF_INET;
    server_client.sin_port = htons(CONN_APP);
    server_client.sin_addr.s_addr = inet_addr(HOST);
    memset(server_client.sin_zero, '\0', sizeof server_client.sin_zero);

    /*Initialize size variable to be used later on*/
    addr_size_client = sizeof server_client;

    /*server layer in ruby*/
    int i;

    /*Create UDP socket*/
    socket_desc = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    server.sin_family = AF_INET;

    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(CONN_NETWORK);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server.sin_zero, '\0', sizeof server.sin_zero);

    /*Bind socket with address struct*/
    bind(socket_desc, (struct sockaddr *) &server, sizeof(server));

    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverStorage;
    /*end server layer in ruby*/

    while(1){

      //received message layer in ruby
      nBytes = recvfrom(socket_desc,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);

      /*Send message to layer in node*/
      sendto(sock_client,buffer,nBytes,0,(struct sockaddr *)&server_client,addr_size_client);

      /*Receive message from server_client in node*/
      nBytes = recvfrom(sock_client,buffer,1024,0,NULL, NULL);

      printf("Received from server_client: %s\n",buffer);

      nBytes = strlen(buffer) + 1;

      /*Send message layer in ruby*/
      sendto(socket_desc,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);

    }

  return 0;
}
