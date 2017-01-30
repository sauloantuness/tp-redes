/************* UDP CLIENT CODE *******************
http://www.programminglogic.com/example-of-client-server_client-program-in-c-using-sockets-and-tcp/
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define CONN_APP 7894
#define CONN_NETWORK 7893
#define HOST "127.0.0.1"

int socket_desc, nBytes, sock_client;
char buffer[3000], message[3000] , server_reply[3000], client_message[3000], data[3000], segment[3000];
struct sockaddr_in server, server_client;
struct sockaddr_storage serverStorage;
socklen_t addr_size, addr_size_client;

void PDU(){
  printf("Port: %d\n",server_client.sin_port);
  printf("Addr: %d\n",server_client.sin_addr.s_addr);
}

int main(){

/*server */
int i;

/*Create UDP socket*/
socket_desc = socket(PF_INET, SOCK_DGRAM, 0);
if (socket_desc == -1)
{
    printf("Could not create socket");
}
puts("Socket created");
/*Configure settings in address struct*/
server.sin_family = AF_INET;

//server.sin_addr.s_addr = htonl(INADDR_ANY);
server.sin_port = htons(CONN_APP);
server.sin_addr.s_addr = inet_addr("127.0.0.1");
memset(server.sin_zero, '\0', sizeof server.sin_zero);

/*Bind socket with address struct*/
  if( bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0)
  {
      //print the error message
      perror("bind failed. Error");
      return 1;
  }
  puts("bind done");

/*Initialize size variable to be used later on*/
addr_size = sizeof serverStorage;

/*end server*/

/*-----------------------client-------------------*/

  /*Create UDP socket application layer*/
  sock_client = socket(PF_INET, SOCK_DGRAM, 0);
  if (sock_client == -1)
  {
      printf("Could not create socket");
  }
  puts("Socket created");
  /*Configure settings in address struct*/
  server_client.sin_family = AF_INET;
  server_client.sin_port = htons(CONN_NETWORK);
  server_client.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(server_client.sin_zero, '\0', sizeof server_client.sin_zero);

  /*Initialize size variable to be used later on*/
  addr_size_client = sizeof server_client;
/*------------end client-------------*/

  while(1){
    //received message browser
    nBytes = recvfrom(socket_desc,buffer,3000,0,(struct sockaddr *)&serverStorage, &addr_size);

    printf("%s\n", buffer);
    nBytes = strlen(buffer) + 1;

    /*Send message to client in ruby*/
    sendto(sock_client,buffer,nBytes,0,(struct sockaddr *)&server_client,addr_size_client);

    /*Receive message from ruby*/
    nBytes = recvfrom(sock_client,buffer,3000,0,NULL, NULL);

    nBytes = strlen(buffer) + 1;

    /*Send message to browser*/
    sendto(socket_desc,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);

    PDU();
  }

  return 0;
}
